#version 330 core



// Vertex
layout (location = 0) in vec3 _position;
layout (location = 1) in vec4 _color;
layout (location = 2) in vec2 _texCoord;

// Fragment
out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentTexCoord;

// Model, view, projection matrixes
uniform mat4 model 		= mat4(1.0);
uniform float positionFactor = 1.0;



// Main shader funciton
void main() {
	fragmentPosition = vec3(model * vec4(_position * positionFactor, 1.0));
    fragmentColor    = _color;
    fragmentTexCoord = _texCoord;
    
	gl_Position = vec4(fragmentPosition, 1.0);
}


