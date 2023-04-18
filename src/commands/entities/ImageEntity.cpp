#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "commands/entities/ImageEntity.h"

bool ImageEntity::programInitialized = false;
QOpenGLShaderProgram* ImageEntity::program = nullptr;

ImageEntity::ImageEntity(const QString &imagePath) {
    texture = new QOpenGLTexture(QImage(QString(imagePath)).mirrored());
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
}

ImageEntity::ImageEntity(const QString &imagePath, float x, float y, float z, bool followPerspective) : ImageEntity(imagePath) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->followPerspective = followPerspective;
}

ImageEntity::~ImageEntity() {
    vbo->destroy();
    delete vbo;
    delete texture;
}

// stupid IDE thinks that program is null
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
void ImageEntity::initProgram([[maybe_unused]] GLWidget &widget) {
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/image.vert");
    auto log = program->log();
    qDebug().noquote() << "Compiling image vertex shader..." << (log.isEmpty() ? "OK" : log);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/image.frag");
    log = program->log();
    qDebug().noquote() << "Compiling image fragment shader..." << (log.isEmpty() ? "OK" : log);
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    log = program->log();
    qDebug().noquote() << "Linking image shader..." << (log.isEmpty() ? "OK" : log);

    program->bind();
    program->setUniformValue("texture", 0);

    programInitialized = true;
}
#pragma clang diagnostic pop

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

    vbo->create();
    vbo->bind();
    vbo->allocate(vertData.constData(), (int) (vertData.count() * sizeof(GLfloat)));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
void ImageEntity::draw(GLWidget &widget) {
    QMatrix4x4 m;

    m.ortho(0, (float) widget.width(), (float) widget.height(), 0, -1000.0f, 1000.0f);
    if (followPerspective && GLWidget::perspective)
        GLWidget::perspective(m, widget, *this);
    if (mode == CORNER) {
        m.translate(x, y, z);
    } else { // CENTER
        m.translate(x - (float) texture->width() / 4.0f, y - (float) texture->height() / 4.0f, z);
    }
    if (mode == CENTER) { m.translate((float) texture->width() / 4.0f, (float) texture->height() / 4.0f, 0); }
    m.scale(xScale, yScale, 1.0f);
    m.rotate((float) xRot, 1.0f, 0.0f, 0.0f);
    m.rotate((float) yRot, 0.0f, 1.0f, 0.0f);
    m.rotate((float) zRot, 0.0f, 0.0f, 1.0f);
    if (followPerspective && GLWidget::postPerspective)
        GLWidget::postPerspective(m, widget, *this);
    if (mode == CENTER) { m.translate(-(float) texture->width() / 4.0f, -(float) texture->height() / 4.0f, 0); }

    program->bind();
    vbo->bind();
    texture->bind();

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    widget.glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
#pragma clang diagnostic pop

bool ImageEntity::isFinished(GLWidget &widget) {
    return false;
}