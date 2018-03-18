// texture
uniform sampler2D texture;

// vignetting
uniform float strength = 1.0;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    pixel.rgb *= strength - distance(vec2(0.5, 0.5), gl_TexCoord[0].xy);

    gl_FragColor = pixel;
}
