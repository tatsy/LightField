varying vec4 P;
uniform sampler2D texture0;

void main(void) {
	vec4 color = texture2DProj(texture0, P);
	gl_FragColor = color;
}
