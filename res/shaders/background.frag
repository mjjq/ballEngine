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

void main() {

    vec4 diffTexel = texture2D(material.diffuseMap, gl_TexCoord[0].xy);


    gl_FragColor = diffTexel;
    //gl_FragColor = vec4(gl_TexCoord[0].xy, 0.0, 1.0);
}
