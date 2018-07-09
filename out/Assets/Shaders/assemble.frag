#version 330 core

in vec2 texCoord;
uniform sampler2D texture;

out vec4 finalColor;



// Main shader function
void main() {
    vec4 pixel = texture2D(texture, texCoord);
    
    if (pixel.x == 0.0) {
        discard;
    }
    else {
        finalColor = pixel;
    }
}


