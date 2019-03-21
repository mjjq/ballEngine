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

struct Light
{
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};


uniform Material material;

#define NR_LIGHTS 10
uniform Light lights[NR_LIGHTS];
uniform sampler2D shadowTextures[NR_LIGHTS];
uniform sampler2D shadowTexture;

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

vec3 calcLight(Light _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir, vec4 _diffTexel)
{
    vec3 relWorldPos = _light.position - _fragPos;
    float lightDistance = distance(_light.position, _fragPos);
    vec3 relWorldUnit = relWorldPos/lightDistance;
    float attenuation = 1.0 / (_light.constant +
                               _light.linear * lightDistance +
                               _light.quadratic * lightDistance * lightDistance);

    float diff = max(dot(_normal, relWorldUnit), 0.0);
    float spec = pow(max(dot(_viewDir, reflect(-relWorldUnit, _normal)), 0.0), material.shininess);

    vec2 shadSize = textureSize(shadowTexture, 0);
    float shadow = texture2D(shadowTexture, vec2(gl_FragCoord.x/float(shadSize.x), gl_FragCoord.y/float(shadSize.y))).b;

    vec3 diffuse = shadow * attenuation * material.diffuseStrength * diff * _light.color * _diffTexel.xyz;
    vec3 ambient = attenuation * material.ambientStrength * _light.color * _diffTexel.xyz;
    vec3 specular = shadow * attenuation * material.specularStrength * spec * _light.color;

    return vec3(diffuse + ambient + specular);
}

void main() {
    vec3 viewDir = vec3(0.0, 0.0, 1.0);


    vec4 diffTexel = texture2D(material.diffuseMap, gl_TexCoord[0].xy);
    vec4 normTexel = texture2D(material.normalMap, gl_TexCoord[0].xy);
    vec4 emitTexel = texture2D(material.emissionMap, gl_TexCoord[0].xy);

    vec3 normal = convToNormal( normTexel );
    normal = vec3(rotate(normal.xy), normal.z);

    vec3 lightColor = vec3(0.0);

    for(int i=0; i<NR_LIGHTS; i++)
    {
        lightColor += calcLight(lights[i], normal, FragPos, viewDir, diffTexel);
    }

    vec3 emission = material.emissionStrength * emitTexel.xyz;

    gl_FragColor = vec4(lightColor + emission, 1.0);
}
