#version 330

smooth in  vec4 position;
smooth in  vec2 textureCoords;
out vec4 color;

uniform sampler2D texture;
uniform int rows;
uniform int cols;
uniform float cameraPositionX;
uniform float cameraPositionY;
uniform float focalLength;
uniform float apertureSize;

void main(void) {
		float spanX = 1.0 / cols;
		float spanY = 1.0 / rows;
		float cameraIndexX = cameraPositionX * (cols - 1);
		float cameraIndexY = cameraPositionY * (rows - 1);
		float gapRatio = 8.0;

		float cameraGapX = gapRatio / (cols - 1);
		float cameraGapY = gapRatio / (rows - 1);
		float initCameraX = -cameraGapX * (cols - 1) * 0.5;
		float initCameraY = -cameraGapY * (rows - 1) * 0.5;
		float focalRatio = 10.0 * gapRatio;

		float centerCameraX = initCameraX + cameraIndexX * cameraGapX;
		float centerCameraY = initCameraY + cameraIndexY * cameraGapY;
		float focalLengthRatio = 1.0 + focalLength / focalRatio;

		color = vec4(0.0, 0.0, 0.0, 0.0);
		int  validPixelCount = 0;
		for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
						float cameraX = initCameraX + float(j) * cameraGapX;
						float cameraY = initCameraY + float(i) * cameraGapY;
						float dx = cameraX - centerCameraX;
						float dy = cameraY - centerCameraY;
						if (dx * dx + dy * dy < apertureSize) {
								float projX   = 2.0 * textureCoords.s - 1.0;
								float projY   = 2.0 * textureCoords.t - 1.0;
								float pixelX = cameraX + (projX - cameraX) * focalLengthRatio;
								float pixelY = cameraY + (projY - cameraY) * focalLengthRatio;
								float px = 0.5 * pixelX + 0.5;
								float py = 0.5 * pixelY + 0.5;
								if(px >= 0.0 && py >= 0.0 && px < 1.0 && py < 1.0) {
										vec2 V;
										V.x = float(j) * spanX + px * spanX;
										V.y = float(i) * spanY + py * spanY;
										color = color + texture2D(texture, V);
										validPixelCount++;
								}
					  }
				}
		}
		color = color / validPixelCount;
}
