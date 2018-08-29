#version 330 core



// Fragment
in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;

// Color output
out vec4 finalColor;

// Texture
uniform bool hasTexture = false;
uniform sampler2D texture;



// Main shader function
void main() {
    if (hasTexture) {
        finalColor = fragmentColor * texture2D(texture, fragmentTexCoord);
    }
    else {
        finalColor = fragmentColor;
    }
}


