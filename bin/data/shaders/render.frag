#version 120

uniform sampler2DRect capTex;

varying vec2 v_TexCoord;

void main() {
    vec4 color = texture2DRect( capTex, v_TexCoord );
    gl_FragColor = color;
}
