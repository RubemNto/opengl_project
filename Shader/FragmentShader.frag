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
	float cutoffAngle;

	float constant;
    float linear;
    float quadratic;

	float power;
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
in vec4 fragmentPosition;

in mat4 modelMatrix;
in mat4 viewMatrix;
in mat4 projectionMatrix;

uniform sampler2D texSampler;
uniform Material material;
uniform vec3 viewDir;

uniform AmbientLight aLight;
uniform DirectionalLight dLight;
uniform PointLight pLight;
uniform SpotLight sLight;

vec3 GetAmbientLight(AmbientLight ambLight)
{
	return material.Ka * ambLight.ambient * ambLight.power * ambLight.state;
}

vec3 GetDirectionalLight(DirectionalLight dirLight,vec3 normal, vec3 viewDir)
{
	normal = normalize(normal);
	vec3 lightDir = normalize(-dirLight.orientation);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.Ns);

	vec3 ambient = dirLight.ambient * 			material.Ka;
	vec3 diffuse = dirLight.diffuse * diff * 	material.Kd;
	vec3 specular = dirLight.specular * spec * 	material.Ks;
	return (ambient + diffuse + specular) * dirLight.power * dirLight.state;
}

vec3 GetPointLight(PointLight pointLight,vec3 normal, vec3 fragPos, vec3 viewDir)
{
	normal = normalize(normal);
	vec3 lightDir = normalize(pointLight.position - vec3(fragPos));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Ns);

    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));    
    
	vec3 ambient  = pointLight.ambient * material.Ka;
    vec3 diffuse  = pointLight.diffuse * diff * material.Kd;
    vec3 specular = pointLight.specular * spec * material.Ks; 
    return (ambient + diffuse + specular) * attenuation * pointLight.power * pointLight.state;
}

vec3 GetSpotLight(SpotLight spotLight,vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(spotLight.position - vec3(fragPos));
	float circleInnerAngle = dot(lightDir,normalize(-spotLight.orientation));

	if(circleInnerAngle < spotLight.cutoffAngle)
	{
		PointLight pointLight;
		pointLight.ambient = spotLight.ambient;
		pointLight.diffuse = spotLight.diffuse;
		pointLight.specular = spotLight.specular;

		pointLight.constant = spotLight.constant;
		pointLight.linear = spotLight.linear;
		pointLight.quadratic = spotLight.quadratic;

		pointLight.power = spotLight.power;
		pointLight.state = spotLight.state;
		
		return GetPointLight(pointLight,normal,fragPos,viewDir);
	}
	else
	{
		return spotLight.ambient * spotLight.power/10.0f * spotLight.state;
	}
}

void main()
{

	vec4 light = vec4(
		GetAmbientLight(aLight)
		+
		GetDirectionalLight(dLight,vec3(projectionMatrix * viewMatrix *  normalVector),viewDir)
		+
		GetPointLight(pLight, vec3(projectionMatrix * viewMatrix * normalVector), vec3(viewMatrix*modelMatrix * fragmentPosition),viewDir)
		+
		GetSpotLight(sLight, vec3(projectionMatrix * viewMatrix * normalVector), vec3(viewMatrix*modelMatrix * fragmentPosition),viewDir)
		,1);
	FragColor = texture(texSampler,textureCoord) * light ;

	// vec4 light = vec4(
	// 	GetAmbientLight(aLight)
	// 	+
	// 	GetDirectionalLight(dLight,vec3(projectionMatrix * viewMatrix *  normalVector),viewDir)
	// 	+
	// 	GetPointLight(pLight, vec3(projectionMatrix * viewMatrix * normalVector), vec3(viewMatrix*modelMatrix * fragmentPosition),viewDir)
	// 	+
	// 	GetSpotLight(sLight, vec3(projectionMatrix * viewMatrix * normalVector), vec3(viewMatrix*modelMatrix * fragmentPosition),viewDir)
	// 	,1);
	// FragColor = texture(texSampler,textureCoord) * light ;
}