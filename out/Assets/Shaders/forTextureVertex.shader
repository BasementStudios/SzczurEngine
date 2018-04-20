#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;

void main() 
{
	gl_Position = vec4(aPos, 0.0, 1.0);
	texCoord = aTexCoord;
}