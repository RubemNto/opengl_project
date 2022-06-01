#version 440 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 textureCoord;
layout (location = 1) in vec4 normalVector;
layout (location = 2) in flat int lightState;

uniform sampler2D TexSampler;

void main()
{
	if(lightState>0)
	{
		FragColor = texture(TexSampler,textureCoord);
	}
	else
	{
		FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
	}
}