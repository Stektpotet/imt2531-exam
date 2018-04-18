#shader vertex
#version 410

// material properties

struct MaterialProperties {

    uniform float opacity;
} mat;

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
out vec3 f_normal;
out vec2 f_uv;
out vec4 f_color;
out vec3 f_position;


////STANDARD UNIFORMS 
// simple uniforms 

uniform float time;

// unifirm blocks / uniform buffer objects
//https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout
layout(std140, row_major) uniform Camera {
    vec3 position;
    mat4 m_viewProjection;
} camera;




void main()
{
    ///// DIFFUSION
    
    //for(int i = 0; i < no_of_lights; i++)
    //    lightToFrag = f_position - light[i].position
    //    intensity = dot(normal, lightToFrag)
    
    //intensity = angle of attack = % of diffusion
    //brightness related to cos x

}