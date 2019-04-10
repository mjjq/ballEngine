varying vec3 worldPos;
varying vec4 TexCoords;

void main()
{
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;

    gl_Position = gl_ProjectionMatrix * vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;

    worldPos = vertex.xyz;
    TexCoords = gl_TexCoord[0];
}
