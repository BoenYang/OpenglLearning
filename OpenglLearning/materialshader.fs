#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shiniess;
};

struct Light{
	vec3 position;
	vec3 diffuse;
};

uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform float ambientStrength;

uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoord;

void main()
{
	
	vec3 n = normalize(Normal);
	vec3 lightDir = normalize(light.position - WorldPos);
	
	vec3 ambient = ambientColor * texture(material.diffuse,TexCoord).rgb * ambientStrength;

	float diff = max(dot(n,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).rgb * diff * light.diffuse;

	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir,n);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shiniess);
	vec3 specular = texture(material.specular,TexCoord).rgb * spec * light.diffuse;
	
	
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result,1.0);
}