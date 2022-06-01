#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) uniform mat4 MVP;
layout(location = 2) in vec2 vertexTexturePosition;

out vec2 textureCoord;
 
void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1.0f);
    textureCoord = vertexTexturePosition;
}