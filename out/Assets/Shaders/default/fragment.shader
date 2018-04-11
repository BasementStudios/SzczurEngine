#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform bool isTextured;

void main() 
{
	if(isTextured) {
		vec4 color = texture(ourTexture, texCoord);
		if(color.a < 0.1) 
			discard;
		FragColor = color;
	}
	else
		FragColor = vec4(ourColor, 1.0);
}

