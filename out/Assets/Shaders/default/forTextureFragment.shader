#version 330 core

uniform sampler2D texture;

uniform vec3 colorMod = vec3(0.7, 0.7, 0.75);
uniform float contrast = 0.5;
uniform float brightness = 0.5;
uniform float gamma = 0.5;

out vec4 FragColor;

in vec2 texCoord;

void main() {
    vec4 pixel = texture2D(texture, texCoord);

    pixel.rgb *= colorMod;

    pixel.rgb = (pixel.rgb - 0.5) * tan((contrast + 1.0) * 0.785398163397448) + 0.5;

    if(brightness < 0.0)
        pixel.rgb *= 1.0 + brightness;
    else
        pixel.rgb += (1.0 - pixel.rgb) * brightness;

    pixel.rgb = pow(pixel.rgb, vec3(1.0 / gamma));

    FragColor = pixel;
}