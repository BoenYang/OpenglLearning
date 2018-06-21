#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	float shiniess;
};

struct DirLight{
	vec3 direction;
	vec3 color;
};

uniform Material material;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform float ambientStrength;
uniform DirLight dirLight;


vec3 CaluateDirLight(DirLight light,vec3 normal,vec3 viewDir);

void main()
{   
	vec3 n = normalize(Normal);
	vec3 viewDir = normalize(viewPos - WorldPos);

	vec3 ambient = ambientColor * texture(material.texture_diffuse1,TexCoord).rgb * ambientStrength;

	vec3 result = CaluateDirLight(dirLight,n,viewDir);
	result += ambient;
    FragColor = vec4(result,1);
}

vec3 CaluateDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = texture(material.texture_diffuse1,TexCoord).rgb * diff * light.color;

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shiniess);
	vec3 specular = texture(material.texture_specular1,TexCoord).rgb * spec * light.color;

	return diffuse + specular;
}
