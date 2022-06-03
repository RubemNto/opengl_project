#version 440 core

// struct AmbientLight{
// 	vec3 color;
// 	float power;
// };

// struct DirectionalLight{
// 	vec3 color;
// 	vec3 direction;
// 	float power;
// };

// struct PointLight{
// 	vec3 position;
// 	vec3 color;
// 	float power;
// };

// struct SpotLight{
// 	vec3 position;
// 	vec3 direction;
// 	vec3 color;
// 	float power;
// 	float cutoffAngle;
// };

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 textureCoord;
layout (location = 1) in vec4 normalVector;
layout (location = 2) in flat int lightState;
layout (location = 3) in vec3 fragPosition;

uniform sampler2D TexSampler;

uniform float Ns;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;


void main()
{
	vec4 lightDir = vec4(0,-1,0,1);
	vec3 ambientLight = Ka;
	
	vec3 difuseLight = Kd * max(dot(normalVector,normalize(lightDir)),0.0f);
	vec3 camPos = vec3(0.0f, 2.0f, 5.0f);
	vec3 camVector = camPos - fragPosition;//V->
	vec4 offsetVector = (normalize(vec4(camVector,1)) + normalVector)/2.0f;//R->
	vec3 specularLight = Ks * pow(max(dot(normalize(vec4(camVector,1)),offsetVector),0.0f),Ns);
	
	
	vec4 result = vec4(ambientLight + difuseLight + specularLight,1);
	FragColor = texture(TexSampler,textureCoord) * result;
}