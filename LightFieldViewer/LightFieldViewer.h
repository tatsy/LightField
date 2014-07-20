#ifndef _LIGHT_FIELD_VIEWER_H_
#define _LIGHT_FIELD_VIEWER_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

class LightFieldViewer {

private:
	static GLuint textureId;
	static float  focal;
	static float  aperture;

	GLuint vShaderId;
	GLuint fShaderId;
	static GLuint programId;
	
	static const int WIDTH  = 600;
	static const int HEIGHT = 600;

public:
	static LightFieldViewer& getInstance(int* argcp, char** argv);
	static void update();

	void show();
	void loadTex(std::string file);

private:
	LightFieldViewer(int* argcp, char** argv);		// default constructor	

	static void keyboard(unsigned char key, int x, int y);
	static void display();

	void initGlut();

	void initGlsl();

};

#endif
