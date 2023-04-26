#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImageReader>

#include "commands/entities/ImageEntity.h"

bool ImageEntity::programInitialized = false;
QOpenGLShaderProgram* ImageEntity::program = nullptr;

ImageEntity::ImageEntity(const QString &imagePath) {
    QImageReader image(imagePath);

    if (!image.canRead()) {
        qDebug() << "Failed loading image" << imagePath << QImageReader::supportedImageFormats();
        return;
    }

    if (image.supportsAnimation()) {
        frames = image.imageCount();
        delay = (float) image.nextImageDelay() / 1000.0f;
    }
    textures = new QOpenGLTexture*[frames];

    for (int i = 0; i < frames; i++) {
        QImage img = image.read();
        textures[i] = new QOpenGLTexture(img.mirrored());
        textures[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        textures[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }

    vao = new QOpenGLVertexArrayObject();
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
}

ImageEntity::ImageEntity(const QString &imagePath, float x, float y, float z, bool followPerspective) : ImageEntity(imagePath) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->followPerspective = followPerspective;
}

ImageEntity::~ImageEntity() {
    if (vbo != nullptr){
        vbo->destroy();
        delete vbo;
        vbo = nullptr;
    }

    if (vao != nullptr){
        vao->destroy();
        delete vao;
        vao = nullptr;
    }

    if (textures != nullptr) {
        for (int i = 0; i < frames; i++) {
            if (textures[i] != nullptr) {
                delete textures[i];
                textures[i] = nullptr;
            }
        }

        delete textures;
        textures = nullptr;
    }
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
        // assume all textures are the same size
        vertData.append((float) textures[0]->width() * coord[j][0]);
        vertData.append((float) textures[0]->height() * coord[j][1]);
        vertData.append(coord[j][2]);
        // texture coordinate
        vertData.append(j == 0 || j == 3);
        vertData.append(j == 0 || j == 1);
    }

    vao->create();
    vao->bind();

    vbo->create();
    vbo->bind();
    vbo->allocate(vertData.constData(), (int) (vertData.count() * sizeof(GLfloat)));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
void ImageEntity::draw(GLWidget &widget) {
    // handle gif textures
    timeSinceLastAnimatedFrame += widget.getTimeDelta();
    int frameAdvance = (int) (timeSinceLastAnimatedFrame / delay);
    if (frameAdvance > 0) {
        timeSinceLastAnimatedFrame -= (float) frameAdvance * delay;
        frame = (frame + frameAdvance) % frames;
    }
    QOpenGLTexture* texture = textures[frame];

    QMatrix4x4 m;

    // I am so sorry if you're reading this but
    // - this transformation matrix goes from right to left (i.e., extra operations are implied to be RHS)
    // - we do rotations first (since those need to be at the origin)
    // - then we do the fancy translations
    // extra stuff if we decide to set origin wrt center, versus top left corner

    m.ortho(0, (float) widget.width(), (float) widget.height(), 0, -1000.0f, 1000.0f);
    if (followPerspective && GLWidget::postPerspective)
        GLWidget::postPerspective(m, widget, *this);
    if (mode == CORNER) {
        m.translate(x, y, z);
    } else { // CENTER
        m.translate(x - (float) texture->width() / 2.0f, y - (float) texture->height() / 2.0f, z);
    }
    if (mode == CENTER) { m.translate((float) texture->width() / 2.0f, (float) texture->height() / 2.0f, 0); }
    if (followPerspective && GLWidget::perspective)
        GLWidget::perspective(m, widget, *this);
    m.rotate((float) xRot, 1.0f, 0.0f, 0.0f);
    m.rotate((float) yRot, 0.0f, 1.0f, 0.0f);
    m.rotate((float) zRot, 0.0f, 0.0f, 1.0f);
    m.scale(xScale, yScale, 1.0f);
    if (mode == CENTER) { m.translate(-(float) texture->width() / 2.0f, -(float) texture->height() / 2.0f, 0); }

    program->bind();
    vao->bind();
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

float ImageEntity::getWidth() const {
    return (float) textures[0]->width() * xScale;
}

float ImageEntity::getHeight() const {
    return (float) textures[0]->height() * yScale;
}
