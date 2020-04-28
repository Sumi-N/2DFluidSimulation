#version 430 core

out vec4 color;

in vec2 texcoord;

//uniform sampler2D read;
layout(binding = 0) uniform sampler2D read;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0, 1080.0);

uniform vec2 power;

uniform vec2 point;
//uniform float radius;
const float radius = 2.0f;

float gauss(vec2 p, float r)
{
    return exp(-dot(p, p) / r);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;
    vec2 base = texture2D(read, uv).xy;
    vec2 coord = point.xy - gl_FragCoord.xy;
    vec2 splat = vec2(0, 0);
    splat = power * gauss(coord, gridSize.x * radius);
    color = vec4(base + splat, 0.0, 1.0);
    //color = vec4(base, 0.0 ,1.0);
}
