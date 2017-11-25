#version 330

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texcoords;

out vec4 position;
out vec2 textureCoords;

void main(void) {
    gl_Position = vec4(vertices, 1.0);
    position = gl_Position;
    textureCoords = texcoords;
}
