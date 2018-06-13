#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = texture(texture1,TexCoord);
	//FragColor = vec4(ourColor,1.0);
}