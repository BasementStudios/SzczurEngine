// texture
uniform sampler2D texture;

// color_correction
uniform vec3 color_mod = vec3(1.0, 1.0, 1.0);
uniform float contrast = 0.0;
uniform float brightness = 0.0;
uniform float gamma = 1.0;

#define pi4 0.785398163397448

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // color_mod
    pixel.rgb *= color_mod;

    // contrast
    pixel.rgb = (pixel.rgb - 0.5) * tan((contrast + 1.0) * pi4) + 0.5;

    // brightness
    if(brightness < 0.0)
        pixel.rgb *= 1.0 + brightness;
    else
        pixel.rgb += (1.0 - pixel.rgb) * brightness;

    // gamma
    pixel.rgb = pow(pixel.rgb, vec3(1.0 / gamma));

    gl_FragColor = pixel;
}
