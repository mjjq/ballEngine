struct Material
{
    sampler2D diffuseMap;
    sampler2D normalMap;
    sampler2D emissionMap;

    float diffuseStrength;
    float ambientStrength;
    float specularStrength;
    float emissionStrength;
    float shininess;
};

uniform Material material;

uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform float rotCosine;
uniform float rotSine;

varying vec3 FragPos;

vec3 convToNormal(vec4 pixel)
{
    vec3 norm = normalize(2.0*pixel.rgb - 1.0);
    norm.y = -norm.y;
    return norm;
}

vec2 rotate(vec2 inputVec)
{
    return vec2(rotCosine*inputVec.x - rotSine*inputVec.y,
                rotSine*inputVec.x + rotCosine*inputVec.y);
}

void main() {
    vec3 viewDir = vec3(0.0, 0.0, 1.0);


    vec4 diffTexel = texture2D(material.diffuseMap, gl_TexCoord[0].xy);
    vec4 normTexel = texture2D(material.normalMap, gl_TexCoord[0].xy);
    vec4 emitTexel = texture2D(material.emissionMap, gl_TexCoord[0].xy);

    vec3 normal = convToNormal( normTexel );
    normal = vec3(rotate(normal.xy), normal.z);
    vec3 relWorldPos = lightPosition - FragPos;
    vec3 relWorldUnit = normalize(relWorldPos);


    float diff = max(dot(normal, relWorldUnit), 0.0);
    float spec = pow(max(dot(viewDir, reflect(-relWorldUnit, normal)), 0.0), material.shininess);

    vec3 diffuse = material.diffuseStrength * diff * lightColor;
    vec3 ambient = material.ambientStrength * lightColor;
    vec3 specular = material.specularStrength * spec * lightColor;
    vec3 emission = material.emissionStrength * emitTexel.xyz;

    gl_FragColor = vec4((diffuse + ambient) * diffTexel.xyz + specular + emission, 1.0);
}
