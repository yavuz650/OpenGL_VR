#version 330 core
precision mediump float;
uniform highp sampler2D main_tex;
in mediump vec2 uv;
out lowp vec4 outcolor;
void main() {
        outcolor = texture(main_tex, uv);
}
