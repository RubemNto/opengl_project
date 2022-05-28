#version 440 core

layout( location = 0 ) in vec3 vertexPosition;
layout( location = 1 ) in vec2 vertexTextureCoordinate;
layout( location = 2 ) uniform mat4 mvp;

out vec2 textureCoordinate;

void main()
{
    gl_Position = mvp * vec4(vertexPosition, 1.0f);
    textureCoordinate = vertexTextureCoordinate;
}