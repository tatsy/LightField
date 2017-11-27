#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;

out vec4 f_position;
out vec2 f_texcoord;

void main(void) {
    gl_Position = vec4(in_position, 1.0);
    f_position = gl_Position;
    f_texcoord = in_texcoord;
}
