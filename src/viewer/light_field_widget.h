#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_FIELD_WIDGET_H
#define LIGHT_FIELD_WIDGET_H

#include <string>
#include <vector>
#include <memory>

#include <qopenglwidget.h>
#include <qopenglbuffer.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglfunctions.h>
#include <qopenglshaderprogram.h>
#include <qopengltexture.h>
#include <qtimer.h>
#include <qevent.h>
#include <qpoint.h>

#include "imageinfo.h"

class LightFieldWidget : public QOpenGLWidget, private QOpenGLFunctions {
    Q_OBJECT

public:
    explicit LightFieldWidget(QWidget* parent = nullptr);
    virtual ~LightFieldWidget();

    void setLightField(const std::vector<ImageInfo>& viewInfos, int rows, int cols);
    void setFocusPoint(float value);
    void setApertureSize(float value);
    float focusPoint() const;
    float apertureSize() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;

protected slots:
    void animate();

private:
    std::unique_ptr<QOpenGLVertexArrayObject> vao;
    std::unique_ptr<QOpenGLBuffer> vbo;
    std::unique_ptr<QOpenGLBuffer> ibo;

    std::unique_ptr<QOpenGLShaderProgram> shaderProgram;
    std::unique_ptr<QOpenGLTexture> lightFieldTexture;

    std::unique_ptr<QTimer> timer;

    float focus, aperture;
    int lfRows, lfCols;
    QSize imageSize;
    QPoint prevMouseClick;
    QPointF cameraPosition;
    bool isClick;
};

#endif  // LIGHT_FIELD_WIDGET_H
