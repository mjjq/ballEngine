layout (lines) in;

layout (triangle_strip, max_vertices = 4) out;

uniform vec3 lightPos;
uniform float rayLength;

in vec3 worldPos[];
in vec4 TexCoords[];

varying vec4 TexCoord;

void constructFan(int index,
                  vec3 _lightPos,
                  float _rayLength)
{
    vec3 rayDir1 = worldPos[index] - lightPos;
    vec3 rayDir2 = worldPos[index+1] - lightPos;

    vec3 newPos1 = worldPos[index] + _rayLength*rayDir1;
    vec3 newPos2 = worldPos[index+1] + _rayLength*rayDir2;

    float trapWide = length(newPos2 - newPos1);
    vec3 trapVectorNarrow = worldPos[index+1] - worldPos[index];
    float trapNarrow = length(trapVectorNarrow);

    gl_Position = gl_in[index].gl_Position;
    TexCoord = trapNarrow * vec4(TexCoords[0].x, 0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_ProjectionMatrix * vec4(newPos1, 1.0);
    TexCoord = trapWide * vec4(TexCoords[0].x, 1.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[index+1].gl_Position;
    TexCoord = trapNarrow * vec4(TexCoords[1].x, 0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_ProjectionMatrix * vec4(newPos2, 1.0);
    TexCoord = trapWide * vec4(TexCoords[1].x, 1.0, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}


// Main entry point
void main()
{
    constructFan(0, lightPos, rayLength);
    EndPrimitive();

}
