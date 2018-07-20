#version 330 core



// Fragment
in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;

// Color output
out vec4 finalColor;

// Texture
uniform sampler2D texture;



// Main shader function
void main() {
    vec4 pixel = texture2D(texture, fragmentTexCoord);

    if (pixel.a == 0.0) {
        discard;
    }
    else {
        finalColor = pixel;
    }
}
