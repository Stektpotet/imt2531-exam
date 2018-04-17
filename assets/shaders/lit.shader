#shader vertex
#version 410

// material properties




//struct definitions
struct Light {
	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 intensities;
};

// vertex attributes
layout(location = 0) in vec4 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;
layout(location = 3) in vec4 v_color;

// out attributes
out vec3 f_normal
out vec2 f_uv;
out vec4 f_color;


////STANDARD UNIFORMS 
// simple uniforms 

uniform float time;

// uniform buffer objects

uniform lightBlock{
	Light lights[4
];