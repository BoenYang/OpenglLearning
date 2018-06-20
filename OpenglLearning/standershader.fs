#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

struct Material{
	sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.texture_diffuse1, TexCoord);
}