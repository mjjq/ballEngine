uniform float zPosition;
uniform vec2 windowSize;
uniform vec2 viewCentre;

void main()
{
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;

    vertex.xy -= viewCentre/abs(zPosition);
    vertex.xy /= windowSize;

    vertex.y *= -1.0f;
    gl_Position = vertex;


    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
