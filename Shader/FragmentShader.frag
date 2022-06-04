#version 440 core

struct AmbientLight{
	vec3 ambient;
	float power;
	int state;
};

struct DirectionalLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 orientation;
	float power;
	int state;
};

struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
    float linear;
    float quadratic;

	vec3 position;
	float power;
	int state;
};

struct SpotLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	vec3 orientation;
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

out vec4 FragColor;

in vec2 textureCoord;
in vec4 normalVector;
in mat4 modelMatrix;
in mat4 viewMatrix;
in mat4 projectionMatrix;

uniform sampler2D texSampler;
uniform Material material;
uniform vec3 viewDir;

uniform AmbientLight aLight;
uniform DirectionalLight dLight;
uniform PointLight pLight;
// uniform SpotLight sLight;

vec3 GetAmbientLight(AmbientLight ambLight)
{
	return material.Ka * ambLight.ambient * ambLight.power * ambLight.state;
}

vec3 GetDirectionalLight(DirectionalLight dirLight,vec3 normal, vec3 viewDir)
{
	normal = normalize(vec3(projectionMatrix * viewMatrix * normalVector));
	vec3 lightDir = normalize(-dirLight.orientation);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.Ns);

	vec3 ambient = dirLight.ambient;
	vec3 diffuse = dirLight.diffuse * diff;
	vec3 specular = dirLight.specular * spec;
	return (ambient + diffuse + specular) * dirLight.power;
}

void main()
{
	vec4 light = vec4(
		GetAmbientLight(aLight)
		+
		GetDirectionalLight(dLight,vec3(normalVector),viewDir)
		,1);
	FragColor = texture(texSampler,textureCoord) * light ;
}