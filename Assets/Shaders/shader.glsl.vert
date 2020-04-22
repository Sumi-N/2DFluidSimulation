#version 430 core

layout (location = 0) in vec3 model_position;
layout (location = 1) in vec2 model_texcoord;

//layout(binding = 0) uniform sampler2D texture;

out vec2 texcoord;

void main()
{
    gl_Position = vec4(model_position, 1.0);
    //gl_Position = texture2D(texture,);
    texcoord = model_texcoord;
}