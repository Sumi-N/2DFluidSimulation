#version 430 core

out vec4 color;

uniform sampler2D read;

uniform vec3 bias;
uniform vec3 scale;

varying vec2 texCoord;

void main()
{
    color = vec4(bias + scale * texture2D(read, texCoord).xyz, 1.0);
}
