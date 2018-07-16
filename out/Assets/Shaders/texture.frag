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
    finalColor = fragmentColor * texture2D(texture, fragmentTexCoord);
}


