#shader vertex
#version 410 core

layout(location = 0) in vec4 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;
layout(location = 3) in vec4 v_vertex_color_from_program;

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
    mat4 translation = mat4(1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,0);

    mat4 rot = mat4(0.525322, 0.000000, -0.850904, 0.000000,
                    0.000000, 1.000000, 0.000000, 0.000000,
                    0.850904, 0.000000, 0.525322, 0.000000,
                    0.000000, 0.000000, 0.000000, 1.000000);

    float F = sqrt(v_position.x*v_position.x + v_position.y*v_position.y + v_position.z*v_position.z);
    gl_Position = projection * translation * view * rotate(time*0.1*F, time*0.333334*F, time*0.1666666667*F) * v_position;
}

#shader fragment
#version 410 core

in vec4 pos;
out vec4 out_color;

void main() {
    out_color = vec4(.5, .5, 1, 1);
}
