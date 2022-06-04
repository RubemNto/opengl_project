#version 440 core

in vec3 vertexPosition;
in vec2 vertexTexturePosition;
in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool deform;
uniform float sinTime;

out vec2 textureCoord;
out vec4 normalVector;
out vec4 fragmentPosition;
out mat4 modelMatrix;
out mat4 viewMatrix;
out mat4 projectionMatrix;

void main()
{
    if(deform)
    {
        gl_Position = projection * view * (model + mat4(sinTime+1)) * vec4(vertexPosition+vec3(0,sinTime,0), 1.0f);
        fragmentPosition =  projection*view*(model + mat4(sinTime+1))*vec4(vertexPosition, 1.0f);
    }else
    {
        gl_Position = projection*view*model * vec4(vertexPosition, 1.0f);
        fragmentPosition =  projection*view*model*vec4(vertexPosition, 1.0f);
    }
    textureCoord = vertexTexturePosition;
    normalVector =  projection*view*model*vec4(vertexNormal,1.0f); 
    modelMatrix = model;
    viewMatrix = view;
    projectionMatrix = projection;
}