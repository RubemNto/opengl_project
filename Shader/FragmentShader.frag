#version 440 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 textureCoord;
uniform sampler2D TexSampler;

void main()
{
	FragColor = texture(TexSampler,textureCoord);
}