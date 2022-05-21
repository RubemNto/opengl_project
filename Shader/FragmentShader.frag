#version 440 core

out vec4 FragColor;

in vec3 influenceColor;
in vec2 textureCoordinate;

uniform sampler2D textureImage;

void main()
{
	FragColor = texture(textureImage,textureCoordinate);
}