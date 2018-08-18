#version 330 core



// Fragment
in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;

// Color output
out vec4 finalColor;



// Main shader function
void main() {
    finalColor = fragmentColor;
}


