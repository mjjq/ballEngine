varying vec3 FragPos;

void main()
{
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;

    gl_Position = gl_ProjectionMatrix * vertex;

    FragPos = vertex.xyz;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
