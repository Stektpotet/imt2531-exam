#prop Cull off
#prop Blend off
#prop ZTest off
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex _color;

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

#define PI 3.1415926535897932384626433832795

void main() {


    vec2 pos = vec2(frag_Pos.xy*noiseScale);

    // Use the noise function
    float n = noise(pos);


    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(0.5*-sin(PI+dayTime*PI*2)+0.5, 0.5)).rgb;
    vec3 sunDirection = vec3(cos(PI+dayTime*PI*2),sin(PI+dayTime*PI*2), 0);

    vec3 fragDirection = -frag_Pos;

    float size = 4;
    float sunPower = pow(max(dot(normalize(fragDirection), normalize(sunDirection)), 0.0), 512) * size;
    vec3 sun = dayTimeSunColor * sunPower;
    vec3 tex = texture(skyTexture, texCoords).rgb*intensity;
    color = vec4(tex*n+sun, 1);
}
