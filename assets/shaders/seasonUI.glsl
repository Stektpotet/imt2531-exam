#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out vec3 frag_Pos;
out vec2 texCoord;

#define PI 3.1415926535897932384626433832795

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

mat4 rotateX(in float rad) {
	return mat4(
		1, 0,		  0,		0,
		0, cos(rad), -sin(rad), 0,
		0, sin(rad),  cos(rad), 0,
		0, 0,		  0,		1
	);
}


vec4 MVP(in vec4 position) {
    return projection * view * m2w * rotateX(seasonTime*2*PI) * position;
}

void main() {
    texCoord = uv;
    vec4 out_position = MVP(position);
    gl_Position = out_position;
    frag_Pos = vec3(m2w * position);
}

#shader fragment
#version 410

in vec2 texCoord;
in vec3 frag_Pos;
out vec4 out_color;

uniform sampler2D mainTex;

void main() {
	vec3 tex = texture(mainTex, texCoord).rgb;
    out_color = vec4(tex, 1);
}
