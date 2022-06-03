#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexturePosition;
layout(location = 3) in vec3 vertexNormal;


layout (location = 1) uniform mat4 MVP;
layout (location = 2) uniform mat4 ModelView;	
layout (location = 3) uniform mat3 NormalMatrix;

out vec2 textureCoord;
out vec4 normalVector;
out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1.0f);
    textureCoord = vertexTexturePosition;
    normalVector = vec4(vertexNormal,1.0f); 

	vPositionEyeSpace = (ModelView * vec4(vertexPosition, 1.0)).xyz;
	vNormalEyeSpace = normalize(NormalMatrix * vertexNormal);
}