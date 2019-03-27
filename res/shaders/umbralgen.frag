vec3 colorA = vec3(0.0,0.0,0.0);
vec3 colorB = vec3(1.0,1.0,1.0);

in vec4 TexCoord;

uniform float lightWidthL;
uniform float lightWidthR;

void main() {
    vec2 st = TexCoord/TexCoord.w;
    vec3 color = vec3(0.0);

    vec2 pos = vec2(1.0,0.0);
    float widthL = lightWidthL;
    float widthR = lightWidthR;

    float yL = widthL*pow(st.y, 1.0);
    float yR = widthR*pow(st.y, 1.0);
    float x = smoothstep(pos.y+yL, pos.y, st.x);
    float x2 = smoothstep(pos.x-yR, pos.x, st.x);
    float x3 = x+x2;

    vec3 pct = vec3(x3);

    //color = mix(colorA, colorB, pct);

    gl_FragColor = vec4(colorA, 1.0-pct);
    //y = pow(st.y, 1.0);// + distance(st.x, 0.5);
    //gl_FragColor = vec4(st.y, 1.0-st.y, 0.0, 1.0);
    //gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
