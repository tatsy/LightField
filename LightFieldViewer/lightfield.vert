varying vec4 P;

void main(void) {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	P = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
