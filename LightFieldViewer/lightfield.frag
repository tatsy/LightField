varying vec4 P;
uniform sampler2D texture0;

uniform int rows;
uniform int cols;
uniform float focalLength;
uniform float apertureSize;

void main(void) {
	float focal = 15.0f;
	float W = 1.0f / cols;
	float H = 1.0f / rows;

	float gapRatio = 8.5f;
	float cameraGapX = gapRatio / (cols - 1);
	float cameraGapY = gapRatio / (rows - 1);
	float initCameraX = -cameraGapX * (cols - 1) / 2.0f;
	float initCameraY = -cameraGapY * (rows - 1) / 2.0f;
	float focalRatio = 10.0f * gapRatio;

	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	int  validPixelCount = 0;
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			float cameraX = initCameraX + j * cameraGapX;
			float cameraY = initCameraY + i * cameraGapY;
			float radius = cameraX * cameraX + cameraY * cameraY;
			if(radius > apertureSize * apertureSize) continue;

			float projX   = 0.5f + P.x - 1.0 / 2.0f;
			float projY   = 0.5f + P.y - 1.0 / 2.0f;
			float pixelX = cameraX + (projX - cameraX) / (1.0f + focalLength / focalRatio);
			float pixelY = cameraY + (projY - cameraY) / (1.0f + focalLength / focalRatio);
			float px = pixelX; // + 1.0f / 2.0f;
			float py = pixelY; // + 1.0f / 2.0f;
			if(px >= 0.0f && py >= 0.0f && px < 1.0f && py < 1.0f) {
				vec4 V = P;
				V.x = i * W + px * W;
				V.y = j * H + py * H;
				color = color + texture2DProj(texture0, V);
				validPixelCount++;
			}
		}
	}
	gl_FragColor = color / validPixelCount;
}
