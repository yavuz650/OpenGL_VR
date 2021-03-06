#version 330 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform mat4 u_modelview;
uniform mat4 u_projection;
out mediump vec2 uv;

void main() {
    gl_Position = u_projection * u_modelview * vec4(in_position,1.0);
    uv = in_uv;
}
