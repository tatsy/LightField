#version 150

in vec3 vertices;
in vec2 texcoords;

smooth out vec4 position;
smooth out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;

void main(void) {
		gl_Position = projectionMatrix * modelviewMatrix * vec4(vertices, 1.0);
		position  = modelviewMatrix * vec4(vertices, 1.0);
		textureCoords = texcoords;
}
