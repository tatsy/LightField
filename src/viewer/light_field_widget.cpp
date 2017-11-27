#include "light_field_widget.h"

#include <iostream>
#include <fstream>

#include <QtGui/qmatrix4x4.h>
#include <QtGui/qvector3d.h>

#include "directories.h"

static constexpr uint32_t indexBufferSize = 6;

struct Vertex {
    QVector3D position;
    QVector2D texcoord;
};

LightFieldWidget::LightFieldWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , focus(0.0f)
    , aperture(5.0f)
    , lfRows(0)
    , lfCols(0)
    , imageSize(0, 0)
    , cameraPosition(0.5f, 0.5f)
    , isClick(false) {
    // Setup timer
    timer = std::make_unique<QTimer>(this);
    timer->start(10);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(animate()));
}

LightFieldWidget::~LightFieldWidget() {
}

QSize LightFieldWidget::sizeHint() const {
    int ratio = devicePixelRatio();
    int w = std::max(1024, imageSize.width()) / ratio;
    int h = std::max(1024, imageSize.height()) / ratio;
    return QSize(w, h);
}

QSize LightFieldWidget::minimumSizeHint() const {
    return sizeHint();
}

void LightFieldWidget::initializeGL() {
    // Initialize OpenGL functions
    initializeOpenGLFunctions();

    // Clear background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shader programs
    const QString vShaderFile = QString(SHADER_DIRECTORY) + "lightfield.vert";
    const QString fShaderFile = QString(SHADER_DIRECTORY) + "lightfield.frag";
    shaderProgram = std::make_unique<QOpenGLShaderProgram>(this);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vShaderFile);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fShaderFile);
    shaderProgram->link();

    if (!shaderProgram->isLinked()) {
        std::cerr << "[ERROR: failed to link shader program!!" << std::endl;
        exit(1);
    }

    // Setup VAO for image plane
    const std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
        0u, 1u, 2u, 0u, 2u, 3u
    };

    vao = std::make_unique<QOpenGLVertexArrayObject>(this);
    vao->create();
    vao->bind();

    vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo->bind();
    vbo->allocate(vertices.data(), sizeof(Vertex) * vertices.size());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3 ,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

    ibo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
    ibo->create();
    ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo->bind();
    ibo->allocate(indices.data(), sizeof(uint32_t) * indices.size());

    vao->release();
}

void LightFieldWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void LightFieldWidget::paintGL() {
    // If texture is not loaded, then return.
    if (!lightFieldTexture) return;

    // Clear buffers
	glClear(GL_COLOR_BUFFER_BIT);

    // Enable shader
    shaderProgram->bind();

    // Set camera parameters
    shaderProgram->setUniformValue("focusPoint", focus);
    shaderProgram->setUniformValue("apertureSize", aperture);
    shaderProgram->setUniformValue("cameraPositionX", (float)cameraPosition.x());
    shaderProgram->setUniformValue("cameraPositionY", (float)cameraPosition.y());

    // Set texture parameters
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lightFieldTexture->textureId());
    shaderProgram->setUniformValue("textureImages", 0);
    shaderProgram->setUniformValue("focusPoint", focus);
    shaderProgram->setUniformValue("apertureSize", aperture);
    shaderProgram->setUniformValue("rows", lfRows);
    shaderProgram->setUniformValue("cols", lfCols);

    vao->bind();
    glDrawElements(GL_TRIANGLES, indexBufferSize, GL_UNSIGNED_INT, 0);
    vao->release();

    lightFieldTexture->release(0);
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
    lfRows = rows;
    lfCols = cols;
    makeCurrent();

    // Check image size of each view
    QImage temp(viewInfos[0].path());
    if (temp.isNull()) {
        qDebug("[ERROR] failed to load image: %s",
               viewInfos[0].path().toStdString().c_str());
        std::abort();
    }
    const int imageW = temp.width();
    const int imageH = temp.height();
    imageSize = QSize(imageW, imageH);

    // Create large tiled image
    std::vector<uint8_t> imageData((rows * cols) * imageH * imageW * 4, 255);
    for (int k = 0; k < viewInfos.size(); k++) {
        QImage img(viewInfos[k].path());
        if (img.width() != imageW || img.height() != imageH) {
            qDebug("[ERROR] image size invalid!!");
            std::abort();
        }

        for (int y = 0; y < imageH; y++) {
            for (int x = 0; x < imageW; x++) {
                const QColor color = img.pixelColor(x, y);
                imageData[((k * imageH + y) * imageW + x) * 4 + 0] = 127; //(uint8_t)color.red();
                imageData[((k * imageH + y) * imageW + x) * 4 + 1] = 127; //(uint8_t)color.green();
                imageData[((k * imageH + y) * imageW + x) * 4 + 2] = 0; //(uint8_t)color.blue();
            }
        }
    }

//    lightFieldTexture = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target3D);
    lightFieldTexture = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target2D);
    lightFieldTexture->setAutoMipMapGenerationEnabled(true);
    lightFieldTexture->setMinMagFilters(QOpenGLTexture::Filter::Linear, QOpenGLTexture::Filter::Linear);
    lightFieldTexture->setWrapMode(QOpenGLTexture::CoordinateDirection::DirectionS,
                                   QOpenGLTexture::WrapMode::ClampToEdge);
    lightFieldTexture->setWrapMode(QOpenGLTexture::CoordinateDirection::DirectionT,
                                   QOpenGLTexture::WrapMode::ClampToEdge);
    lightFieldTexture->setWrapMode(QOpenGLTexture::CoordinateDirection::DirectionR,
                                   QOpenGLTexture::WrapMode::ClampToEdge);
    lightFieldTexture->setFormat(QOpenGLTexture::TextureFormat::RGBA8_UNorm);
//    lightFieldTexture->setSize(imageW, imageH, rows * cols);
    lightFieldTexture->setSize(imageW * cols, imageH * rows);
    lightFieldTexture->allocateStorage(QOpenGLTexture::PixelFormat::RGBA,
                                       QOpenGLTexture::PixelType::UInt8);
    lightFieldTexture->setData(0, 0, QOpenGLTexture::PixelFormat::RGBA,
                               QOpenGLTexture::PixelType::UInt8, imageData.data());

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
