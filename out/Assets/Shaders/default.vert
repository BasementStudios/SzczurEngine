#version 330 core

// Vertex from OpenGL
layout (location = 0) in vec3 _position;
layout (location = 1) in vec4 _color;
layout (location = 2) in vec2 _texCoord;

// Output for fragment shader
out vec3 position;
out vec4 color;
out vec2 texCoord;

// MVP matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float positionFactor;

void main()
{
    gl_Position = projection * view * model * vec4(_position * positionFactor, 1.0);

    position = _position;
    color = _color;
    texCoord = _texCoord;
}
