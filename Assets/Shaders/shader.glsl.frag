#version 430 core

out vec4 color;

in vec2 texcoord;

layout(binding = 0) uniform sampler2D texture;

void main()
{
    //color = vec4(1.0, 0, 0, 1.0);
    color = texture2D(texture, texcoord);
}