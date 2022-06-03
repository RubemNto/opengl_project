#version 440 core

struct AmbientLight{
	vec3 color;
	float power;
	int state;
};

struct DirectionalLight{
	vec3 color;
	vec3 orientation;
	float power;
	int state;
};

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
	int state;
};

struct SpotLight{
	vec3 position;
	vec3 orientation;
	vec3 color;
	float power;
	float cutoffAngle;
	int state;
};

struct Material
{
	float Ns;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

// color to be exported
layout (location = 0) out vec4 FragColor;

//Model information
layout (location = 0) in vec2 textureCoord;
layout (location = 1) in vec4 normalVector;
layout (location = 2) in vec3 vPositionEyeSpace;
layout (location = 3) in vec3 vNormalEyeSpace;

uniform sampler2D TexSampler;
uniform Material material;

uniform mat4 Model;
uniform mat4 View;

uniform AmbientLight aLight;
uniform DirectionalLight dLight;
// uniform PointLight pLight;
// uniform SpotLight sLight;

vec3 GetAmbientLight(AmbientLight ambLight)
{
	return material.Ka * ambLight.color * ambLight.power * ambLight.state;
}

vec3 GetDirectionalLight(DirectionalLight dirLight)
{
	vec3 ambient = material.Ka * (dirLight.color* dirLight.power);
	
	vec3 lightDirectionEyeSpace = (View * vec4(dirLight.orientation, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec3 diffuse = material.Kd * vec3(1,1,1)* dirLight.power * NdotL;

	vec3 V = normalize(-vPositionEyeSpace);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec3 specular = pow(RdotV, material.Ns) * vec3(1,1,1)* dirLight.power * material.Ks;

	return (ambient + diffuse + specular) * dirLight.state;
}

vec3 GetPointLight(PointLight pointLight)
{
	vec3 ambient = material.Ka * pointLight.color;

	vec3 lightPositionEyeSpace = (View * vec4(pointLight.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec3 diffuse = material.Kd * vec3(1,1,1)* pointLight.power * NdotL;

	vec3 V = normalize(-vPositionEyeSpace);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.Ns) * vec3(1,1,1)* pointLight.power * material.Ks
	
	float dist = length(mat3(View) * pointLight.position - vPositionEyeSpace);	
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (attenuation * (ambient + diffuse + specular));
}

vec3 GetSpotLight(SpotLight spotLight)
{
	return vec3(0,0,0);
}

void main()
{
	vec4 light = vec4(
		GetAmbientLight(aLight) + 
		GetDirectionalLight(dLight),1);
	FragColor = texture(TexSampler,textureCoord) * light ;
}