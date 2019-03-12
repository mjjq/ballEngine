uniform sampler2D texture;
uniform sampler2D normalMap;

uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform float rotCosine;
uniform float rotSine;

varying vec3 FragPos;

vec3 convToNormal(vec4 pixel)
{
    return normalize(2.0*pixel.rgb - 1.0);
}

vec2 rotate(vec2 inputVec)
{
    return vec2(rotCosine*inputVec.x - rotSine*inputVec.y,
                rotSine*inputVec.x + rotCosine*inputVec.y);
}

void main() {
    float ambientStrength = 0.3;
    float specularStrength = 0.5;
    vec3 viewDir = vec3(0.0, 0.0, -1.0);


    vec4 diffTexel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 normTexel = texture2D(normalMap, gl_TexCoord[0].xy);

    vec3 normal = -convToNormal( normTexel );
    normal.y = -normal.y;
    normal = vec3(rotate(normal.xy), normal.z);

    vec3 relWorldPos = -lightPosition + FragPos;
    float dist = distance(lightPosition, FragPos);

    float diff = max(dot(normal, normalize(relWorldPos)), 0.0);

    vec4 lightVec4 = vec4(diff * lightColor, 1.0);

    gl_FragColor = diffTexel * lightVec4 + ambientStrength*diffTexel;
}
