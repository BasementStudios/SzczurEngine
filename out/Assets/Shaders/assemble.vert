#version 330 core



// Vertex
layout (location = 0) in vec3 _position;
layout (location = 2) in vec2 _texCoord;

out vec2 texCoord;



// Main shader funciton
void main() {
    texCoord = _texCoord;
	
	gl_Position = vec4(_position.xy, 0.0, 1.0);
}


