varying vec3 worldPos;

varying vec4 color;

varying vec2 viewSize;

void main()
{
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;

    gl_Position = gl_ProjectionMatrix * vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;

    color = gl_Color;

    vec2 size = vec2(12.0);

    viewSize = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(gl_Vertex.xy + size, gl_Vertex.zw)).xy;
    worldPos = gl_Position.xyz;
    viewSize -= worldPos.xy;
}
