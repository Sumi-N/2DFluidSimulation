#version 430 core

out vec4 color;

in vec2 texcoord;

//uniform sampler2D p;
//uniform sampler2D w;
layout(binding = 0) uniform sampler2D p;
layout(binding = 1) uniform sampler2D w;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0f, 1080.0f);
//uniform float gridScale;
const float gridScale = 1.0f;

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;

    vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
    vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

    float pl = texture2D(p, uv - xOffset).x;
    float pr = texture2D(p, uv + xOffset).x;
    float pb = texture2D(p, uv - yOffset).x;
    float pt = texture2D(p, uv + yOffset).x;

    float scale = 0.5 / gridScale;
    vec2 gradient = scale * vec2(pr - pl, pt - pb);

    vec2 wc = texture2D(w, uv).xy;

    color = vec4(wc - gradient, 0.0, 1.0);
}
