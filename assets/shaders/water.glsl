#prop Cull off
#prop Blend SrcAlpha OneMinusSrcAlpha
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

uniform mat4 m2w;
layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

layout(std140) uniform OK_Times{
	float time;
	float seasonTime;
	float dayTime;
	float tideTime;
};

vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}



////////////////////////////////////////
//PERLIN NOISE AS IMPLEMENTED IN:
//  https://thebookofshaders.com/11/
////////////////////////////////////////

// 2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners porcentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

uniform float noiseScale = 1;


out vec3 frag_Pos;
out vec2 texCoords;
out vec3 fragNormal;

void main() {


	vec2 pos = vec2(position.xy*noiseScale);

    // Use the noise function
    float n = noise(pos);

	fragNormal = normalize(vec3(sin(n+vertex_color.r), 1, cos(n+vertex_color.b)));

	texCoords = uv;
    vec4 transformedPos = MVP(position+vec4(0,0.5*sin(n + time)+16,0,0));
    frag_Pos = vec3(position);
    gl_Position = transformedPos;
}


//==================================================================================
//==================================================================================
//==================================================================================
//==================================================================================
//==================================================================================
//==================================================================================

#shader fragment
#version 410

in vec3 frag_Pos;
in vec2 texCoords;
in vec3 fragNormal;


out vec4 color;

layout(std140) uniform OK_Times{
	float time;
	float seasonTime;
	float dayTime;
	float tideTime;
};

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

uniform sampler2D dayTimeSunColorRamp;
uniform sampler2D seasonSunIntensityRamp;
uniform float specularity = 1;
uniform float intensity = 1;

#define MAX_LIGHTS 8
#define EPSILON 0.0000000000000001
#define PI 3.1415926535897932384626433832795

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
    vec3 lightDir = normalize(position - frag_Pos);
    float diffusion = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffusion * intensities;

    // Specularity
    vec3 viewDir = normalize(view_position.xyz - frag_Pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularity * specPower *  intensities;

    // Attenuation
    float distance = length(position - frag_Pos);
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
    vec3 viewDir = normalize(view_position.xyz - frag_Pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularity * specPower *  intensities;


	return (ambient + diffuse + specular);
}

void main() {
    // seasonColor =  (s * gray) + seasonColor * (1-s)*0.8;
	// float flatness = min(1, 0.25 + abs(dot(fragNormal, fragUp)));

    float seasonSunIntensity = texture(seasonSunIntensityRamp, vec2(0.45*-cos(dayTime)+0.5, seasonTime)).r;
    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(0.5*-sin(PI+dayTime*PI*2)+0.5, 0.5)).rgb;
    vec3 sunDirection = vec3(cos(PI+dayTime*PI*2),sin(dayTime*PI*2), 0);
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


    vec3 fragDirection = -frag_Pos;

    float size = 4;
    float sunPower = pow(max(dot(normalize(fragDirection), normalize(sunDirection)), 0.0), 7) * size;


    vec3 sun = dayTimeSunColor * sunPower;
    color = vec4(sun * (1 - sin(PI+dayTime*PI*4)), cos(dayTime*PI*4)*0.13);
}
