
#version 430 core

out vec4 color;

in vec2 texcoord;

layout(binding = 0) uniform sampler2D x;
layout(binding = 1) uniform sampler2D b;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0f, 1080.0f);

//uniform float alpha;
const float alpha = 1 / (0.1 * 50);
//const float alpha = 0;
//uniform float beta;
const float beta = 4 + 1 / ( 0.1 * 50);
//const float beta = 4;

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;

    vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
    vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

    vec4 xl = texture2D(x, uv - xOffset);
    vec4 xr = texture2D(x, uv + xOffset);
    vec4 xb = texture2D(x, uv - yOffset);
    vec4 xt = texture2D(x, uv + yOffset);

    vec4 bc = texture2D(b, uv);

    vec4 resultcolor = (xl + xr + xb + xt + alpha * bc) / beta;
    color = vec4(vec3(resultcolor), 1.0);
}