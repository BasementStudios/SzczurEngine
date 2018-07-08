#version 330 core

in vec2 texCoord;
uniform sampler2D texture;

out vec4 finalColor;



// Main shader function
void main() {
    finalColor = texture(texture, texCoord);
}


