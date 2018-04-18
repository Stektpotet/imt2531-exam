#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color_from_program;

out vec2 texCoord;

uniform mat4 projection = mat4(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);

uniform mat4 view = mat4(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);

uniform float time = 0;

mat4 rotate(float x, float y, float z) {
    return mat4(
        (cos(y + z) + cos(y - z)) / 2,                                                                              (-sin(y + z) + sin(y - z)) / 2,                                                                             -sin(y),                        0,
        (cos(x + y + z) - cos(x - y + z) + cos(x + y - z) - cos(x - y - z) + 2 * sin(x + z) - 2 * sin(x - z)) / 4,  (2 * cos(x + z) + 2 * cos(x - z) - sin(x + y + z) + sin(x - y + z) + sin(x + y - z) - sin(x - y - z)) / 4,  (-sin(x + y) - sin(x - y)) / 2, 0,
        (-2 * cos(x + z) + 2 * cos(x - z) + sin(x + y + z) - sin(x - y + z) + sin(x + y - z) - sin(x - y - z)) / 4, (cos(x + y + z) - cos(x - y + z) - cos(x + y - z) + cos(x - y - z) + 2 * sin(x + z) + 2 * sin(x - z)) / 4,  (cos(x + y) + cos(x - y)) / 2,  0,
        0,                                                                                                          0,                                                                                                          0,                              1
    );
}

void main() {

// model to world space transformations = transform
// translation * rotation * scale * vertexPos;

    mat4 translation = mat4(    1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,0,0);

    mat4 rot = mat4(0.525322, 0.000000, -0.850904, 0.000000,
                    0.000000, 1.000000, 0.000000, 0.000000,
                    0.850904, 0.000000, 0.525322, 0.000000,
                    0.000000, 0.000000, 0.000000, 1.000000);

    float F = sqrt(position.x*position.x + position.y*position.y + position.z*position.z);
    gl_Position = projection * translation * view * rotate(time*0.1*F, time*0.333334*F, time*0.1666666667*F) * position;
    texCoord = uv;
}

#shader fragment
#version 410
in vec4 gl_FragCoord;
in vec2 texCoord;
out vec4 out_color;

uniform float time = 0;
uniform sampler2D mainTexture;

void main() {
    out_color = .5*vec4(texture(mainTexture, texCoord).rgb, 1);
}
