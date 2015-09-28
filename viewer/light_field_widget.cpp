#include "light_field_widget.h"

#include <iostream>
#include <fstream>

#include <QtGui/qmatrix4x4.h>
#include <QtGui/qvector3d.h>

#include "directories.h"

LightFieldWidget::LightFieldWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , focus(0.0f)
    , aperture(5.0f)
    , cameraPosition(0.5f, 0.5f)
    , shaderProgram(new QOpenGLShaderProgram)
    , lightFieldTexture(new QOpenGLTexture(QOpenGLTexture::Target::Target2D))
    , timer(new QTimer)
    , isClick(false) {
    // timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
}

LightFieldWidget::~LightFieldWidget() {
    delete shaderProgram;
    delete lightFieldTexture;
    delete timer;
}

void LightFieldWidget::initializeGL() {
    // Clear background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable texture
    glEnable(GL_TEXTURE_2D);

    // Compile and link shader programs
    const QString vShaderFile = QString(kShaderDirectory.c_str()) +
                                "lightfield.vert";
    const QString fShaderFile = QString(kShaderDirectory.c_str()) +
                                "lightfield.frag";
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           vShaderFile);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           fShaderFile);
    shaderProgram->link();

    if (!shaderProgram->isLinked()) {
        std::cerr << "[ERROR: failed to link shader program!!" << std::endl;
        exit(1);
    }
}

void LightFieldWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void LightFieldWidget::paintGL() {
    QMatrix4x4 projectionMatrix, modelviewMatrix;

    // Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Compute matrices
    projectionMatrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    modelviewMatrix.setToIdentity();

    // Enable shader
    makeCurrent();
    shaderProgram->bind();
	
	// Show textured quad
    if (lightFieldTexture->isCreated()) {
        // Set MVP matrix
        shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
        shaderProgram->setUniformValue("modelviewMatrix", modelviewMatrix);

        // Set camera parameters
        shaderProgram->setUniformValue("focusPoint", focus);
        shaderProgram->setUniformValue("apertureSize", aperture);
        shaderProgram->setUniformValue("cameraPositionX", (float)cameraPosition.x());
        shaderProgram->setUniformValue("cameraPositionY", (float)cameraPosition.y());

        // Set attributes
        const QVector3D vertices[4] = {
            QVector3D(-1.0f, -1.0f, 0.0f),
            QVector3D(-1.0f,  1.0f, 0.0f),
            QVector3D( 1.0f,  1.0f, 0.0f),
            QVector3D( 1.0f, -1.0f, 0.0f)
        };

        const QVector2D texcoords[4] = {
            QVector2D(0.0f, 1.0f),
            QVector2D(0.0f, 0.0f),
            QVector2D(1.0f, 0.0f),
            QVector2D(1.0f, 1.0f)
        };

        const unsigned int indices[4] = { 0, 1, 2, 3 };

        shaderProgram->enableAttributeArray("vertices");
        shaderProgram->enableAttributeArray("texcoords");

        shaderProgram->setAttributeArray("vertices", vertices);
        shaderProgram->setAttributeArray("texcoords", texcoords);
        
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices);

        shaderProgram->disableAttributeArray("vertices");
        shaderProgram->disableAttributeArray("texcoords");
    }
}

void LightFieldWidget::mousePressEvent(QMouseEvent* ev) {
    if (ev->button() == Qt::MouseButton::LeftButton) {
        isClick = true;
        cameraPosition.setX((float)ev->x() / width());
        cameraPosition.setY((float)ev->y() / height());
    }
}

void LightFieldWidget::mouseMoveEvent(QMouseEvent* ev) {
    if (ev->buttons() & Qt::MouseButton::LeftButton) {
        if (isClick) {
            cameraPosition.setX((float)ev->x() / width());
            cameraPosition.setY((float)ev->y() / height());
        }
    }
}

void LightFieldWidget::mouseReleaseEvent(QMouseEvent* ev) {
    if (ev->button() == Qt::MouseButton::LeftButton) {
        isClick = false;
    }
}

void LightFieldWidget::setLightField(const std::vector<ImageInfo>& viewInfos,
                                     int rows, int cols) {
    // Check image size of each view
    QImage temp(viewInfos[0].path());
    if (temp.isNull()) {
        qDebug("[ERROR] failed to load image: %s",
               viewInfos[0].path().toStdString().c_str());
        std::abort();
    }
    const int imageW = temp.width();
    const int imageH = temp.height();
    this->resize(imageW, imageH);

    // Create large tiled image
    QImage tiledImage(imageW * rows, imageH * cols, temp.format());
    for (int k = 0; k < viewInfos.size(); k++) {
        const int i = viewInfos[k].row();
        const int j = viewInfos[k].col();
        QImage img(viewInfos[k].path());
        if (img.width() != imageW || img.height() != imageH) {
            qDebug("[ERROR] image size invalid!!");
            std::abort();
        }

        for (int y = 0; y < imageH; y++) {
            for (int x = 0; x < imageW; x++) {
                const QRgb rgb = img.pixel(x, y);
                tiledImage.setPixel(j * imageW + x, i * imageH + y, rgb);
            }
        }
    }
    lightFieldTexture->destroy();

    lightFieldTexture->create();
    lightFieldTexture->setData(tiledImage,
                               QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    lightFieldTexture->setMinificationFilter(QOpenGLTexture::Filter::Linear);
    lightFieldTexture->setMagnificationFilter(QOpenGLTexture::Filter::Linear);
    lightFieldTexture->setWrapMode(QOpenGLTexture::CoordinateDirection::DirectionS,
                                   QOpenGLTexture::WrapMode::ClampToEdge);
    lightFieldTexture->setWrapMode(QOpenGLTexture::CoordinateDirection::DirectionT,
                                   QOpenGLTexture::WrapMode::ClampToEdge);
    lightFieldTexture->bind(0);

    // Set uniform values to the shader
    if (shaderProgram != nullptr) {
        shaderProgram->setUniformValue("texture", 0);
        shaderProgram->setUniformValue("focusPoint", focus);
        shaderProgram->setUniformValue("apertureSize", aperture);
        shaderProgram->setUniformValue("rows", rows);
        shaderProgram->setUniformValue("cols", cols);
    }
    qDebug("[INFO] light field texture is binded!!");
}

void LightFieldWidget::setFocusPoint(float value) {
    focus = value;
}

void LightFieldWidget::setApertureSize(float value) {
    aperture = value;
}

float LightFieldWidget::focusPoint() const {
    return focus;
}

float LightFieldWidget::apertureSize() const {
    return aperture;
}

void LightFieldWidget::animate() {
    update();
}
