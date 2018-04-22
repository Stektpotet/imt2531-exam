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

layout(std140) uniform OK_Matrices {
    mat4 projection;
    mat4 view;
};

layout(std140) uniform OK_Lights {
    vec3 position;
    vec3 intensities;
    float spread;
} lights[8];



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

void main() {

    float F = sqrt(position.x*position.x + position.y*position.y + position.z*position.z);
    mat4 rot = rotate(0, time*F, time);

    vec4 rotatedNormal = rot * vec4(normal, 1);
    // Pass some variables to the fragment shader
    fragNormal = vec3(rotatedNormal);
    fragVert = vec3(position);

    // model to world space transformations = transform
    // translation * rotation * scale * vertexPos;

    mat4 translation = mat4(1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 0);



    gl_Position = projection * view * m2w * rot * position;
    vertex_color_out = rotatedNormal;
    texCoord = uv;
}

#shader fragment
#version 140
in vec4 gl_FragCoord;
in vec2 texCoord;
in vec4 vertex_color_out;
in vec4 pos;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 out_color;

uniform float time = 0;

uniform sampler2D mainTex;
uniform sampler2D bumpTex;

uniform float opacity = 0;
uniform float intensity = 1;
uniform float bumpiness = 0;

//layout(std140) uniform OK_Matrices{
//    mat4 projection;
//    mat4 view;
//};

layout(std140) uniform OK_Lights{
    vec3 position;
    vec3 intensities;
    float spread;
} lights[8];


void main() {

    vec4 tex = texture(mainTex, texCoord);
    vec4 bump = texture(bumpTex, texCoord);
    //out_color = vec4(fragNormal, 1)*0.2 + 0.8*vec4(texture(mainTex, texCoord).rgb , vertex_color_out.a);
	out_color = vec4(tex.rgb * vertex_color_out.rgb, vertex_color_out.a);
}
//out_color = vec4(texture(mainTexture, texCoord).rgb * vertex_color_out.rgb, vertex_color_out.a);
