#version 430 core

out vec4 color;

in vec2 texcoord;
  
//uniform sampler2D velocity;
//uniform sampler2D advected; 
layout(binding = 0) uniform sampler2D graphictexture;
layout(binding = 1) uniform sampler2D velocitytexture;

//uniform vec2 gridSize;
const vec2 gridSize = vec2(1920.0f, 1080.0f);
//uniform float gridScale;
const float gridScale = 1.0f;

//uniform float timestep;
const float timestep = 0.1f;
//uniform float dissipation;
const float dissipation = 1.0f;

vec3 bilerp(sampler2D d, vec2 p)
{
    vec4 ij; // i0, j0, i1, j1
    ij.xy = floor(p - 0.5) + 0.5;
    ij.zw = ij.xy + 1.0;

    vec4 uv = ij / gridSize.xyxy;
    vec3 d11 = texture2D(d, uv.xy).xyz;
    vec3 d21 = texture2D(d, uv.zy).xyz;
    vec3 d12 = texture2D(d, uv.xw).xyz;
    vec3 d22 = texture2D(d, uv.zw).xyz;

    vec2 a = p - ij.xy;

    return mix(mix(d11, d21, a.x), mix(d12, d22, a.x), a.y);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;
    float scale = 1.0 / gridScale;

    // trace point back in time
    vec2 p = gl_FragCoord.xy - timestep * scale * texture2D(velocitytexture, uv).xy;

    vec4 resultcolor = vec4(dissipation * bilerp(graphictexture, p), 1.0);

    color = resultcolor;
}