struct Light
{
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

void main() {
    gl_FragColor = vec4(light.color, 1.0);
}
