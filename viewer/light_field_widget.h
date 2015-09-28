#ifdef _MSC_VER
#pragma once
#endif

#ifndef _LIGHT_FIELD_WIDGET_H_
#define _LIGHT_FIELD_WIDGET_H_

#if defined(_WIN32) || defined(__WIN32__)
    #ifdef LIGHT_FIELD_WIDGET_EXPORT
        #define LIGHT_FIELD_WIDGET_DLL __declspec(dllexport)
    #else
        #define LIGHT_FIELD_WIDGET_DLL __declspec(dllimport)
    #endif
#else
    #define LIGHT_FIELD_WIDGET_DLL
#endif

#include <string>
#include <vector>

#include <QtWidgets/qopenglwidget.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopengltexture.h>
#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtCore/qpoint.h>

#include "image_info.h"

class LIGHT_FIELD_WIDGET_DLL LightFieldWidget : public QOpenGLWidget {
    Q_OBJECT

private:
	float  focal;
	float  aperture;
    QPointF cameraPosition;

    QOpenGLShaderProgram* shaderProgram;
    QOpenGLTexture*       lightFieldTexture;
    QTimer*               timer;

    bool isClick;

public:
    LightFieldWidget(QWidget* parent = nullptr);
    ~LightFieldWidget();

    void setLightField(const std::vector<ImageInfo>& viewInfos, int rows, int cols);
    void setFocalLength(float value);
    void setApertureSize(float value);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;


protected slots:
    void animate();
};

#endif
