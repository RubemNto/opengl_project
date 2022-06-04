#version 440 core

in vec3 vertexPosition;
in vec2 vertexTexturePosition;
in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 textureCoord;
out vec4 normalVector;
out mat4 modelMatrix;
out mat4 viewMatrix;
out mat4 projectionMatrix;

void main()
{
    gl_Position = projection*view*model * vec4(vertexPosition, 1.0f);
    textureCoord = vertexTexturePosition;
    normalVector = vec4(vertexNormal,1.0f); 
    modelMatrix = model;
    viewMatrix = view;
    projectionMatrix = projection;
}