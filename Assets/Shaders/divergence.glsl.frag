#version 430 core

out vec4 color;

in vec2 texcoord;

//uniform sampler2D velocity;
layout(binding = 0) uniform sampler2D velocity;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0f, 1080.0f);
//uniform float gridScale;
const float gridScale = 1.0f;

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;

    vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
    vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

    float vl = texture2D(velocity, uv - xOffset).x;
    float vr = texture2D(velocity, uv + xOffset).x;
    float vb = texture2D(velocity, uv - yOffset).y;
    float vt = texture2D(velocity, uv + yOffset).y;

    float scale = 0.5 / gridScale;
    float divergence = scale * (vr - vl + vt - vb);

    color = vec4(divergence, 0.0, 0.0, 1.0);
}
