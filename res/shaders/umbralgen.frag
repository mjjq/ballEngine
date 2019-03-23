uniform float lightWidth;

vec3 colorA = vec3(0.0,0.0,0.0);
vec3 colorB = vec3(0.0,0.0,0.0);

void main() {
    vec2 st = gl_TexCoord[0].xy;
    vec3 color = vec3(0.0);

    vec2 pos = vec2(1.0,0.0);
    float width = lightWidth;

    float y = width*pow(st.y, 1.0);
    float x = smoothstep(pos.x-y, pos.x, st.x);
    float x2 = smoothstep(pos.y+y, pos.y, st.x);
    float x3 = x+x2;

    vec3 pct = vec3(x3);

    color = mix(colorA, colorB, pct);

    gl_FragColor = vec4(color,1.0-pct);
}
