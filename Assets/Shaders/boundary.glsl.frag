#version 430 core

out vec4 color;

uniform sampler2D read;

uniform vec2 gridSize;
uniform vec2 gridOffset;
uniform float scale;

void main()
{
    vec2 uv = (gl_FragCoord.xy + gridOffset.xy) / gridSize.xy;
    color = vec4(scale * texture2D(read, uv).xyz, 1.0);
}
