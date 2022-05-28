#version 440 core

out vec4 FragColor;

layout(location = 0)in vec2 textureCoordinate;

uniform sampler2D textureImage;

void main()
{
	FragColor = texture(textureImage,textureCoordinate);
}