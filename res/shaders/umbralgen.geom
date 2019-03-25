layout (lines) in;

layout (triangle_strip, max_vertices = 4) out;

uniform vec3 lightPos;
uniform float rayLength;

in vec3 worldPos[];
in vec4 TexCoords[];
varying vec4 TexCoord;

varying float trapDimension;

void constructFan(int index,
                  vec3 _lightPos,
                  float _rayLength)
{

    vec3 rayNorm1 = normalize(worldPos[index] - _lightPos);
    vec3 rayNorm2 = normalize(worldPos[index+1] - _lightPos);
    vec3 newPos1 = worldPos[index] + rayNorm1*_rayLength;
    vec3 newPos2 = worldPos[index+1] + rayNorm2*_rayLength;

    float trapNarrow = length(worldPos[index+1] - worldPos[index]);
    float trapWide = length(newPos2 - newPos1);


    gl_Position = gl_in[index].gl_Position;
    TexCoord = trapNarrow * vec4(TexCoords[0].x, 0.0, 0.0, 1.0);
    trapDimension = trapNarrow;
    EmitVertex();

    gl_Position = gl_ProjectionMatrix * vec4(newPos1, 1.0);
    TexCoord = trapWide * vec4(TexCoords[0].x, 1.0, 0.0, 1.0);
    trapDimension = trapWide;
    EmitVertex();

    gl_Position = gl_in[index+1].gl_Position;
    TexCoord = trapNarrow * vec4(TexCoords[1].x, 0.0, 0.0, 1.0);
    trapDimension = trapNarrow;
    EmitVertex();

    gl_Position = gl_ProjectionMatrix * vec4(newPos2, 1.0);
    TexCoord = trapWide * vec4(TexCoords[1].x, 1.0, 0.0, 1.0);
    trapDimension = trapWide;
    EmitVertex();

    EndPrimitive();
}


// Main entry point
void main()
{
    constructFan(0, lightPos, rayLength);
    EndPrimitive();

}
