#version 330 core



// Vertex
layout (location = 0) in vec3 _position;
layout (location = 1) in vec4 _color;

out vec4 color;



// Main shader funciton
void main() {
	color = _color;
	gl_Position = vec4(_position, 1.0);
}


