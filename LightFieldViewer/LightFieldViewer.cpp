#include "LightFieldViewer.h"

#include <iostream>
#include <fstream>
using namespace std;

#include <opencv2/opencv.hpp>

GLuint LightFieldViewer::textureId = 0;
GLuint LightFieldViewer::programId = 0;
float  LightFieldViewer::focal     = 0.0f;
float  LightFieldViewer::aperture  = 1.0f;

LightFieldViewer& LightFieldViewer::getInstance(int* argcp, char** argv) 
{
	static LightFieldViewer viewer(argcp, argv);
	return viewer;
}

LightFieldViewer::LightFieldViewer(int* argcp, char** argv) :
	vShaderId(0),
	fShaderId(0)
{
	// openglの用意
	glutInit(argcp, argv);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Light Field Viewer");

	// コールバック関数の登録
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	initGlut();

	glewInit();
	if(!glewIsSupported("GL_VERSION_2_0")) {
		cerr << "Cannot use glsl!!" << endl;
		exit(-1);
	}
	initGlsl();
}

void LightFieldViewer::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// モデルビュー行列の設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);

	// テクスチャの有効化
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glBegin(GL_POLYGON);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(0, 0);
	glTexCoord2d(0.0, 1.0); 
	glVertex2d(0, HEIGHT);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(WIDTH, HEIGHT);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(WIDTH, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glutSwapBuffers();
}

void LightFieldViewer::keyboard(unsigned char key, int x, int y)
{
	switch(key) {
	case 0x1b:
		glutExit();
		break;

	case 'u':
		focal = min(1.0f, focal + 0.1f);
		update();
		break;

	case 'd':
		focal = max(-1.0f, focal - 0.1f);
		update();
		break;

	case 'l':
		aperture += 1.0f;
		update();
		break;

	case 's':
		aperture = max(1.0f, aperture - 1.0f);
		update();
		break;
	}
}

void LightFieldViewer::initGlut()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
}

void LightFieldViewer::initGlsl()
{
	// vertex shader
	vShaderId = glCreateShader(GL_VERTEX_SHADER);
	ifstream vertFile("lightfield.vert", ios::in);
	istreambuf_iterator<char> vertBegin(vertFile);
	istreambuf_iterator<char> vertEnd;
	string vertData(vertBegin, vertEnd);
	const char* vertSrc = vertData.c_str();
	vertFile.close();

	int compiled;
	glShaderSource(vShaderId, 1, &vertSrc, 0);
	glCompileShader(vShaderId);
	glGetShaderiv(vShaderId, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE) {
		cerr << "Compilation failed: vertex shader" << endl;
		exit(-1);
	}

	// fragment shader
	fShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	ifstream fragFile("lightfield.frag", ios::in);
	istreambuf_iterator<char> fragBegin(fragFile);
	istreambuf_iterator<char> fragEnd;
	string fragData(fragBegin, fragEnd);
	const char* fragSrc = fragData.c_str();
	fragFile.close();

	glShaderSource(fShaderId, 1, &fragSrc, 0);
	glCompileShader(fShaderId);
	glGetShaderiv(fShaderId, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE) {
		cerr << "Compilation failed: fragment shader" << endl;
		exit(-1);
	}

	// create program object
	programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);

	GLint linked;
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &linked);
	if(linked == GL_FALSE) {
		cerr << "Link error !!" << endl; 
	}

	glUseProgram(programId);

	glUniform1i(glGetUniformLocation(programId, "texture0"), 0);
	glUniform1i(glGetUniformLocation(programId, "rows"), 17);
	glUniform1i(glGetUniformLocation(programId, "cols"), 17);
	glUniform1f(glGetUniformLocation(programId, "focalLength"), focal);
	glUniform1f(glGetUniformLocation(programId, "apertureSize"), aperture);
}

void LightFieldViewer::loadTex(std::string file)
{
	cout << file << endl;
	cv::Mat tex = cv::imread(file, CV_LOAD_IMAGE_COLOR);
	if(tex.empty()) {
		cerr << "Failed to load texture file" << endl;
		exit(1);
	}
	cv::cvtColor(tex, tex, CV_BGR2RGB);
	cout << "Texture file loaded!!" << endl;

	// テクスチャの登録
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.cols, tex.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
	glBindTexture(GL_TEXTURE_2D, 0);
	cout << "Texture binded!!" << endl;
}

void LightFieldViewer::show() 
{
	// 描画の開始
	glutMainLoop();
}

void LightFieldViewer::update()
{
	printf("aperture size: %f\n", aperture);
	printf(" focal length: %f\n", focal);
	glUniform1f(glGetUniformLocation(programId, "focalLength"), focal);
	glUniform1f(glGetUniformLocation(programId, "apertureSize"), aperture);
	glutPostRedisplay();
}
