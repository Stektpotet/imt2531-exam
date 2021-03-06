#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out vec2 texCoord;
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
out vec3 fragUp;
out float terrainHeight;
out float steepness;

const vec3 UP = vec3(0,1,0);

vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}

void main() {

    texCoord = uv;
    fragNormal = mat3(transpose(inverse(m2w))) * normal; //http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
	fragUp = mat3(transpose(inverse(m2w))) * vec3(0,1,0);
    vec4 out_position = MVP(position);
    gl_Position = out_position;

	terrainHeight = position.y;

    fragVert = vec3(m2w * position);
}

#shader fragment
#version 410

#define MAX_LIGHTS 8
#define EPSILON 0.0000000000000001

in vec4 gl_FragCoord;
in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragUp;
in vec3 fragVert;

out vec4 out_color;

uniform float opacity = 1;
uniform float specularity = 1;
uniform float intensity = 1;

uniform sampler2D mainTex;
uniform sampler2D aoTex;


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
    vec3 ao = texture(aoTex, texCoord).rgb;

    //Ambience
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * intensities * ao;

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
    vec3 ao = texture(aoTex, texCoord).rgb;

    //Ambience
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * intensities * ao;

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

layout(std140) uniform OK_Times{
	float time;
	float seasonTime;
	float dayTime;
	float tideTime;
};

in float terrainHeight;
in float steepness;
uniform sampler2D seasonsRamp;
const vec3 gray = vec3(0.35, 0.35, 0.35);

uniform sampler2D seasonSunIntensityRamp;
uniform sampler2D dayTimeSunColorRamp;



#define PI 3.1415926535897932384626433832795



void main() {
    vec3 ao = texture(aoTex, texCoord).rgb;
    vec3 tex = texture(mainTex, texCoord).rgb;

    float s = 0.8-dot(fragNormal, fragUp);
    // s = s*s*s;
    vec3 seasonColor = texture(seasonsRamp, vec2(1.5 * terrainHeight-0.065, seasonTime)).rgb;
    // seasonColor =  (s * gray) + seasonColor * (1-s)*0.8;
	// float flatness = min(1, 0.25 + abs(dot(fragNormal, fragUp)));


    float seasonSunIntensity = texture(seasonSunIntensityRamp, vec2(0.48*-cos(dayTime)+0.5, seasonTime)).r;
    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(0.5*-sin(PI+dayTime*PI*2)+0.5, 0.5)).rgb;


    vec3 sunDirection = vec3(cos(PI+dayTime*PI*2),sin(PI+dayTime*PI *2), 0);

    vec3 lights = OK_DirectionalLight(sunDirection, dayTimeSunColor * seasonSunIntensity * max(0,dot(sunDirection, vec3(0,-1,0))));
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

    // out_color = vec4(heightLine, 1);
    // out_color = vec4(fragNormal, 1);
    // out_color = vec4(lights + tex, 1);
	// out_color = vec4(seasonColor * flatness + lights * flatness , 1);
    // out_color = vec4(lights + tex, 1);
    out_color = vec4((seasonColor + lights) * (tex * 0.5 + 0.5) , 1);


}
