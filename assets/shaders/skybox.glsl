#prop Cull on front
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

vec4 MVP_ish(in vec4 position) {
	mat4 viewRotation = view;

	//cut away the view translation
	viewRotation[3][0] = 0;
	viewRotation[3][1] = 0;
	viewRotation[3][2] = 0;
    return projection * viewRotation * m2w * position;
}

out vec3 frag_Pos;
out vec2 texCoords;
void main() {
	texCoords = uv;
    vec4 transformedPos = MVP_ish(position);
    frag_Pos = vec3(position);
    gl_Position = transformedPos;
}

#shader fragment
#version 410

in vec3 frag_Pos;
in vec2 texCoords;

out vec4 color;
uniform sampler2D skyTexture;
uniform float intensity = 1;

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

#define PI 3.1415926535897932384626433832795

void main() {

    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(0.5*-sin(PI+dayTime*PI*2)+0.5, 0.5)).rgb;
    vec3 sunDirection = vec3(cos(PI+dayTime*PI*2),sin(PI+dayTime*PI*2), 0);

    vec3 fragDirection = -frag_Pos;

    float size = 4;
    float sunPower = pow(max(dot(normalize(fragDirection), normalize(sunDirection)), 0.0), 512) * size;
    vec3 sun = dayTimeSunColor * sunPower;
    vec3 tex = texture(skyTexture, texCoords).rgb*intensity;
    color = vec4(tex+sun, 1);
}
