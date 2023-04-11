#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "entities/ImageEntity.h"

bool ImageEntity::programInitialized = false;
QOpenGLShaderProgram* ImageEntity::program = nullptr;

ImageEntity::ImageEntity(const QString &imagePath) {
    texture = new QOpenGLTexture(QImage(QString(imagePath)).mirrored());
}

ImageEntity::~ImageEntity() {
    vbo.destroy();
    delete texture;
}


void ImageEntity::initProgram(GLWidget &widget) {
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/image.vert");
    qDebug() << "Compiling image vertex shader" << program->log();
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/image.frag");
    qDebug() << "Compiling image fragment shader" << program->log();
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    qDebug() << "Linking image shader" << program->log();

    program->bind();
    program->setUniformValue("texture", 0);
}

void ImageEntity::init(GLWidget &widget) {
    if (!programInitialized) {
        initProgram(widget);
    }

    static const float coord[4][3] = {
        // bottom right, bottom left, top left, top right (CCW order)
        { 1, 0, 0 }, { 0, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 }
    };

    QList<GLfloat> vertData;
    for (int j = 0; j < 4; ++j) {
        // vertex position
        vertData.append((float) texture->width() * coord[j][0] / 2.0f);
        vertData.append((float) texture->height() * coord[j][1] / 2.0f);
        vertData.append(coord[j][2]);
        // texture coordinate
        vertData.append(j == 0 || j == 3);
        vertData.append(j == 0 || j == 1);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), (int) (vertData.count() * sizeof(GLfloat)));
}

void ImageEntity::draw(GLWidget &widget) {
    QMatrix4x4 m;
    m.ortho(0, (float) widget.width(), (float) widget.height(), 0, -1000.0f, 1000.0f);
    m.translate(x, y, z);
    m.scale(xScale, yScale, 1.0f);
    m.rotate((float) xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate((float) yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate((float) zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    texture->bind();
    widget.glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool ImageEntity::isFinished(GLWidget &widget) {
    return false;
}

void ImageEntity::setTranslation(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void ImageEntity::setRotation(float xRot, float yRot, float zRot) {
    this->xRot = xRot;
    this->yRot = yRot;
    this->zRot = zRot;
}

void ImageEntity::setScale(float xScale, float yScale) {
    this->xScale = xScale;
    this->yScale = yScale;
}

float ImageEntity::getZ() {
    return z;
}
