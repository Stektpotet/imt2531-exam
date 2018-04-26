#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out vec2 texCoord;
out vec4 vertex_color_out;
out vec4 pos;

uniform mat4 m2w;
uniform float time = 0;

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};



mat4 rotate(float x, float y, float z) {
    return mat4(
        (cos(y + z) + cos(y - z)) / 2, (-sin(y + z) + sin(y - z)) / 2, -sin(y), 0,
        (cos(x + y + z) - cos(x - y + z) + cos(x + y - z) - cos(x - y - z) + 2 * sin(x + z) - 2 * sin(x - z)) / 4, (2 * cos(x + z) + 2 * cos(x - z) - sin(x + y + z) + sin(x - y + z) + sin(x + y - z) - sin(x - y - z)) / 4, (-sin(x + y) - sin(x - y)) / 2, 0,
        (-2 * cos(x + z) + 2 * cos(x - z) + sin(x + y + z) - sin(x - y + z) + sin(x + y - z) - sin(x - y - z)) / 4, (cos(x + y + z) - cos(x - y + z) - cos(x + y - z) + cos(x - y - z) + 2 * sin(x + z) + 2 * sin(x - z)) / 4, (cos(x + y) + cos(x - y)) / 2, 0,
        0, 0, 0, 1
    );
}

out vec3 fragVert;
out vec3 fragNormal;

vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}


void main() {

    float F = sqrt(position.x*position.x + position.y*position.y + position.z*position.z) * 0.01;
    mat4 rot = rotate(0, time*F, 0);

    vec4 rotatedNormal = rot * vec4(normal, 1);

    // Pass some variables to the fragment shader
    //fragNormal = vec3(rotatedNormal);
    vertex_color_out = rotatedNormal;
    texCoord = uv;
    fragNormal = normalize(mat3(transpose(inverse(m2w))) * normal);

    vec4 out_position = MVP(position);
    gl_Position = out_position;

    fragVert = vec3(m2w * position);
}

#shader fragment
#version 140

#define MAX_LIGHTS 8

in vec4 gl_FragCoord;
in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 out_color;

uniform float time = 0;

uniform sampler2D mainTex;
uniform sampler2D bumpTex;
uniform sampler2D specTex;

uniform float opacity = 0;
uniform float specularity = 1;
uniform float intensity = 1;
uniform float bumpiness = 1;
uniform float distortion = 1;
uniform float scatterPower = 1;
uniform float scatterScale = 1;
uniform float scatterThickness = 1;
uniform float shininess;

uniform mat4 m2w;

layout(std140) uniform OK_Matrices{
    mat4 projection;			//64b
    mat4 view;					//64b
    vec4 view_position;			//16b
};								//144b

struct OK_Light_Directional {
	vec4 direction;				//16b
	vec4 intensities;			//16b
};								//32b

struct OK_Light_Point {			
    vec4 position;				//16b
    vec4 intensities;			//16b
	float constant;				//4b
	float linear;				//4b
	float quadratic;			//4b
	float alignment;			//4b
};								//48b

layout(std140) uniform OK_Lights{
    OK_Light_Point light[MAX_LIGHTS];	//MAX_LIGHTS * 48b
	OK_Light_Directional sun;
};

vec3 OK_PointLight(in vec3 position, in vec3 intensities, in float constant, in float linear, in float quadratic) {
    //Ambience
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * intensities;

    vec3 bump = texture(bumpTex, texCoord).rgb*intensity;
    // Diffussion
    vec3 norm = normalize(fragNormal*bump);
    vec3 lightDir = normalize(position - fragVert);
    float diffusion = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffusion * intensities;
	
    vec3 spec = texture(specTex, texCoord).rgb;

    // Specularity
    vec3 viewDir = normalize(view_position.xyz - fragVert);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularity * specPower *  intensities * spec;

    // Attenuation
    float distance = length(position - fragVert);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
	
	//Fast subsurface scattering
	vec3 halfwayDir = normalize(lightDir + norm * distortion);
	float backLight = pow(clamp(dot(viewDir, -halfwayDir),0.0,1.0), scatterPower) * scatterScale;
	vec3 subSurfaceScatter = backLight * intensities * scatterThickness;

    return (ambient + diffuse + specular + subSurfaceScatter) * attenuation;
}


vec3 OK_DirectionalLight(in vec3 lightDir, in vec3 intensities) {
	//Ambience
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * intensities;

	//Diffuse
    vec3 norm = normalize(fragNormal);
    lightDir = normalize(lightDir);
    float diffusion = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffusion * intensities;

	//Specularity
    vec3 viewDir = normalize(view_position.xyz - fragVert);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularity * specPower *  intensities;

	//SubSurface Scatter
	vec3 halfwayDir = normalize(lightDir + norm * distortion);
	float backLight = pow(clamp(dot(viewDir, -halfwayDir),0.0,1.0), scatterPower) * scatterScale;
	vec3 subSurfaceScatter = backLight * intensities * scatterThickness;

	return (ambient + diffuse + specular + subSurfaceScatter);
}

void main() {

	vec3 lights = OK_DirectionalLight(sun.direction.xyz, sun.intensities.rgb);	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		lights += OK_PointLight(
						light[i].position.xyz, 
						light[i].intensities.rgb,
						light[i].constant,
						light[i].linear,
						light[i].quadratic
					);
	}


    out_color = vec4(lights, opacity);
}
//  distortion: 0.3
//  scatterPower: 10
//  scatterScale: 0.2
