#include "LightFieldViewer.h"

int main(int argc, char** argv) {

	LightFieldViewer viewer = LightFieldViewer::getInstance(&argc, argv);
	viewer.loadTex("../ConstructLightFieldTexture/light_field_texture.png");
	viewer.show();
}
