#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform float specularStrength;

in vec3 Normal;
in vec3 WorldPos;

void main()
{
	
	vec3 n = normalize(Normal);
	vec3 lightDir = normalize(lightPos - WorldPos);
	
	float diff = max(dot(n,lightDir),0.0);
	vec3 diffuse = lightColor * diff;

	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir,n);

	float spec = pow(max(dot(viewDir,reflectDir),0.0),128);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 result = (ambientColor * ambientStrength + diffuse + specular) * objectColor;
	FragColor = vec4(result,1.0);
}