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

out vec3 fragVert;
out vec3 fragNormal;

vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}


void main() {
    texCoord = uv;
    fragNormal = mat3(transpose(inverse(m2w))) * normal; //http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/

    vec4 out_position = MVP(position);
    gl_Position = out_position;

    fragVert = vec3(m2w * position);
}

#shader fragment
#version 410

#define MAX_LIGHTS 8
#define EPSILON 0.0000000000000001

in vec4 gl_FragCoord;
in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 out_color;

uniform float time = 0;

uniform float opacity = 1;
uniform float specularity = 1;
uniform float intensity = 1;

uniform mat4 m2w;

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

struct OK_Light_Directional {
	vec4 direction;
	vec4 intensities;
};

struct OK_Light_Point {
    vec4 position;
    vec4 intensities;
	float constant;
	float linear;
	float quadratic;
	float alignment;
};

layout(std140) uniform OK_Lights{
    OK_Light_Point light[MAX_LIGHTS];
	OK_Light_Directional sun;
};

vec3 OK_PointLight(in vec3 position, in vec3 intensities, in float constant, in float linear, in float quadratic) {
    //Ambience
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * intensities;

    // Diffussion
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(position - fragVert);
    float diffusion = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffusion * intensities;

    // Specularity
    vec3 viewDir = normalize(view_position.xyz - fragVert);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularity * specPower *  intensities;

    // Attenuation
    float distance = length(position - fragVert);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance +EPSILON);


    return (ambient + diffuse + specular) * attenuation;
}


vec3 OK_DirectionalLight(in vec3 lightDir, in vec3 intensities) {
	//Ambience
	float ambientStrength = 0.6;
	vec3 ambient = ambientStrength * intensities;

	//Diffuse
    vec3 norm = normalize(fragNormal);
    lightDir = -normalize(lightDir);
    float diffusion = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffusion * intensities;

	//Specularity
    vec3 viewDir = normalize(view_position.xyz - fragVert);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularity * specPower *  intensities;

	
	return (ambient + diffuse + specular);
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
    out_color = vec4(lights, 1);
}
