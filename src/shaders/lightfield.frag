#version 330

in vec4 position;
in vec2 textureCoords;
out vec4 color;

uniform sampler2D textureImages;
uniform int rows;
uniform int cols;
uniform float cameraPositionX;
uniform float cameraPositionY;
uniform float focusPoint;
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
    float focusRatio = 10.0 * gapRatio;

    float centerCameraX = initCameraX + cameraIndexX * cameraGapX;
    float centerCameraY = initCameraY + cameraIndexY * cameraGapY;
    float focusPointRatio = 1.0 + focusPoint / focusRatio;

    /*
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
                float pixelX = cameraX + (projX - cameraX) * focusPointRatio;
                float pixelY = cameraY + (projY - cameraY) * focusPointRatio;
                float px = 0.5 * pixelX + 0.5;
                float py = 0.5 * pixelY + 0.5;
                if(px >= 0.0 && py >= 0.0 && px < 1.0 && py < 1.0) {
                    vec3 V;
                    V.x = float(j) * spanX + px * spanX;
                    V.y = float(i) * spanY + py * spanY;
                    V.z = float(i * cols + j) / float(rows * cols);
                    color = color + texture(textureImages, V);
                    validPixelCount++;
                }
            }
        }
    }
    color = color / validPixelCount;
    */
    color = texture(textureImages, vec2(textureCoords)); //vec3(0.5, 0.5, 0.5)); //vec3(textureCoords, 0.5));
}
