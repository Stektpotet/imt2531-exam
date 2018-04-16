#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color_from_program;

out vec2 texCoord;
smooth out vec4 vertex_color_out;
out vec4 pos;

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
/*
}
mat4 modelToWorld(mat3 transform) {
    mat4 m = mat4(1); //ORIGINAL model representation
    mat4 scale = mat4(
        transform[0][0], 0,               0,               0,
        0,               transform[0][1], 0,               0,
        0,               0,               transform[0][2], 0,
        0,               0,               0,               1
    );

    m = scale * m;
    m = rotate(transform[1][0], transform[1][1], transform[1][2]) * m;

    mat4 translate = mat4(  1,               0,               0,               0,
                            0,               1,               0,               0,
                            0,               0,               1,               0,
                            transform[2][0], transform[2][1], transform[2][2], 0);
    m = translate * m;
    return m;
}*/

out vec3 fragVert;
out vec3 fragNormal;

void main() {
	// Pass some variables to the fragment shader
	fragNormal = normal;
	fragVert = vec3(position);

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

    //float F = sqrt(position.x*position.x + position.y*position.y + position.z*position.z);
    gl_Position = projection * translation * view * /* rotate(time*0.1*F, time*0.333334*F, time*0.1666666667*F) **/ position;
    vertex_color_out = vertex_color_from_program;
    texCoord = uv;
}

#shader fragment
#version 140
in vec4 gl_FragCoord;
// in vec2 gl_PointCoord; // @NOTE Not supported on [macos, openGL 4.1]
in vec2 texCoord;
smooth in vec4 vertex_color_out;
in vec4 pos;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 out_color;

uniform float time = 0;

uniform sampler2D mainTex;

uniform float test = 0;

uniform struct Light {
	vec3 position;
	vec3 intensities; //a.k.a the color of the light
} light = Light(vec3(1,10,10), vec3(1,1,1));

uniform mat4 model = mat4(1);

void main() {
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	//calculate the location of this fragment (pixel) in world coordinates
	vec3 fragPosition = vec3(model * vec4(fragVert, 1));

	//calculate the vector from this pixels surface to the light source
	vec3 surfaceToLight = light.position - fragPosition;

	//calculate the cosine of the angle of incidence
	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
	brightness = clamp(brightness, 0, 1);

	vec4 surfaceColor = texture(mainTex, texCoord);
	out_color = vec4(brightness * light.intensities * surfaceColor.rgb, surfaceColor.a);

    //out_color = vec4(texture(mainTex, texCoord).rgb , vertex_color_out.a);
}
//out_color = vec4(texture(mainTex, texCoord).rgb * vertex_color_out.rgb, vertex_color_out.a);
