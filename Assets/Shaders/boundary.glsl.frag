#version 430 core

out vec4 color;

//uniform sampler2D read;
layout(binding = 0) uniform sampler2D read;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0f, 1080.0f);
uniform vec2 gridOffset;
//uniform float scale;
cost float scale = 1.0f;

void main()
{
    vec2 uv = (gl_FragCoord.xy + gridOffset.xy) / gridSize.xy;
    color = vec4(scale * texture2D(read, uv).xyz, 1.0);
}
