#version 430 core

in vec3 fColor;

out vec3 FragColor;

uniform vec3 lightColor;



void main()
{
	FragColor = fColor;
}