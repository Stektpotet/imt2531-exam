#prop Cull on back
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out vec3 frag_Pos;

uniform mat4 m2w;
layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};
vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}

void main() {
    vec4 out_position = MVP(position);
    gl_Position = out_position;
    frag_Pos = vec3(m2w * position);
}

#shader fragment
#version 410

in vec3 frag_Pos;
out vec4 out_color;

uniform float time = 0;

void main() {
	float pink = sin(time+30*length(frag_Pos))*0.4+0.6;
    out_color = vec4(pink,0,pink, 1);
}