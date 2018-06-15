#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shiniess;
};

struct DirLight{
	vec3 direction;
	vec3 diffuse;
};

struct PointLight{
	vec3 position;
	vec3 diffuse;

	float constant;
	float linear;
	float quadratic;
};


struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 diffuse;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

};

uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform float ambientStrength;

uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoord;

vec3 CaluateDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CaluatePointLight(PointLight light,vec3 normal,vec3 worldPos, vec3 viewDir);
vec3 CaluateSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir);

void main()
{
	
	vec3 n = normalize(Normal);
	vec3 viewDir = normalize(viewPos - WorldPos);

	vec3 ambient = ambientColor * texture(material.diffuse,TexCoord).rgb * ambientStrength;

	vec3 result = CaluateDirLight(dirLight,n,viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CaluatePointLight(pointLights[i],n,WorldPos,viewDir);
	}
	
	result += CaluateSpotLight(spotLight,n,WorldPos,viewDir);
	result = ambient + result;
	FragColor = vec4(result,1.0);
}

vec3 CaluateDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).rgb * diff * light.diffuse;

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shiniess);
	vec3 specular = texture(material.specular,TexCoord).rgb * spec * light.diffuse;

	return diffuse + specular;
}

vec3 CaluatePointLight(PointLight light,vec3 normal,vec3 worldPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - worldPos);


	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).rgb * diff * light.diffuse;

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shiniess);
	vec3 specular = texture(material.specular,TexCoord).rgb * spec * light.diffuse;

	float distance = length(light.position - worldPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	return (diffuse + specular) * attenuation;
}

vec3 CaluateSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - worldPos);
	float theta = dot(lightDir,normalize(-light.direction));


	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).rgb * diff * light.diffuse;

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shiniess);
	vec3 specular = texture(material.specular,TexCoord).rgb * spec * light.diffuse;
		
	float epsilon  = light.cutOff - light.outerCutOff;
	float indensity = clamp((theta - light.outerCutOff) / epsilon,0.0,1.0);

	float distance = length(light.position - worldPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
		
	return (diffuse + specular) * indensity * attenuation;
	
	
}