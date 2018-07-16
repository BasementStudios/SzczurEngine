#version 330 core



// Vertex
layout (location = 0) in vec3 _position;
layout (location = 1) in vec4 _color;
layout (location = 2) in vec2 _texCoord;

out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentTexCoord;

// Model, view, projection matrixes
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float positionFactor;



// Main shader function
void main()
{
    fragmentPosition = vec3(model * vec4(_position * positionFactor, 1.0));
    fragmentColor    = _color;
    fragmentTexCoord = _texCoord;
    
    gl_Position = (projection * view * vec4(fragmentPosition, 1.0));
}
