#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexturePosition;
layout(location = 3) in vec3 vertexNormal;


layout (location = 1) uniform mat4 MVP;
layout (location = 2) uniform int lightEnabled;

out vec2 textureCoord;
out vec4 normalVector;
out vec3 fragPosition;
out int lightState;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1.0f);
    textureCoord = vertexTexturePosition;
    normalVector = vec4(vertexNormal,1.0f); 
    lightState = lightEnabled;
    fragPosition = vertexPosition;
}