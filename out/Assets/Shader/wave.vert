// Wave
uniform float phase = 1.0;
uniform vec2 amplitude = vec2(1.0, 1.0);

void main()
{
    vec4 vertex = gl_Vertex;

    vertex.x += cos(gl_Vertex.y * 0.02 + phase * 3.8) * amplitude.x
              + sin(gl_Vertex.y * 0.02 + phase * 6.3) * amplitude.x * 0.3;
    vertex.y += sin(gl_Vertex.x * 0.02 + phase * 2.4) * amplitude.y
              + cos(gl_Vertex.x * 0.02 + phase * 5.2) * amplitude.y * 0.3;

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
