#ifdef _MSC_VER
#pragma once
#endif

#ifndef _LIGHT_FIELD_VIEWER_H_
#define _LIGHT_FIELD_VIEWER_H_

#if defined(_WIN32) || defined(__WIN32__)
    #ifdef LIGHT_FIELD_VIEWER_EXPORT
        #define LIGHT_FIELD_VIEWER_DLL __declspec(dllexport)
    #else
        #define LIGHT_FIELD_VIEWER_DLL __declspec(dllimport)
    #endif
#else
    #define LIGHT_FIELD_VIEWER_DLL
#endif

#include <qopenglwidget.h>
#include <string>

class LIGHT_FIELD_VIEWER_DLL LightFieldViewer : public QOpenGLWidget {
    Q_OBJECT

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
    LightFieldViewer(QWidget* parent = NULL);
    ~LightFieldViewer();

    void loadTex(std::string file);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;


private:

	static void keyboard(unsigned char key, int x, int y);

	void initGlut();
	void initGlsl();
};

#endif
