#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect capTex;

varying vec2 v_TexCoord;

void main() {
    vec4 color = texture2DRect( capTex, v_TexCoord );
    gl_FragColor = color;
}
