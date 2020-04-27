#version 430 core

out vec4 color;

in vec2 texcoord;
  
//uniform sampler2D velocity;
//uniform sampler2D advected; 
layout(binding = 0) uniform sampler2D velocitytexture;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0f, 1080.0f);
//uniform float gridScale;
const float gridScale = 1.0f;

//uniform float timestep;
const float timestep = 0.1f;
//uniform float dissipation;
const float dissipation = 1.0f;

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;

    if(uv.x> 0.49 &&  uv.x < 0.51 && uv.y> 0.49 && uv.y < 0.51)
    {
        color = vec4(100, 100, 0, 1.0);
    }else
    {
        color = texture2D(velocitytexture, uv);
    }
}