#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "commands/entities/TextEntity.h"
#include "Fonts.h"

bool TextEntity::programInitialized = false;
QOpenGLShaderProgram* TextEntity::program = nullptr;

TextEntity::TextEntity(const std::string& font, const std::string& text, int size) {
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->font = font;
    this->text = text;
    this->size = size;
    this->color = Qt::red;
}

TextEntity::TextEntity(const std::string& font, const std::string &text, int size, float x, float y, float z) : TextEntity(font, text, size) {
    this->x = x;
    this->y = y;
    this->z = z;
}

TextEntity::~TextEntity() {
    vbo->destroy();
    delete vbo;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
void TextEntity::initProgram([[maybe_unused]] GLWidget &widget) {
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/text.vert");
    auto log = program->log();
    qDebug().noquote() << "Compiling text vertex shader..." << (log.isEmpty() ? "OK" : log);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/text.frag");
    log = program->log();
    qDebug().noquote() << "Compiling text fragment shader..." << (log.isEmpty() ? "OK" : log);
#define PROGRAM_VERTEX_ATTRIBUTE 0
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->link();
    log = program->log();
    qDebug().noquote() << "Linking text shader..." << (log.isEmpty() ? "OK" : log);

    program->bind();
    program->setUniformValue("texture", 0);

    programInitialized = true;
}
#pragma clang diagnostic pop

void TextEntity::init(GLWidget &widget) {
    if (!programInitialized) {
        initProgram(widget);
    }

    Fonts::getInstance().loadFont(widget, font, size);
    vbo->create();
    vbo->bind();
    vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vbo->allocate(sizeof(float) * 6 * 4);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
void TextEntity::draw(GLWidget &widget) {
    QMatrix4x4 m;

    m.ortho(0, (float) widget.width(), 0, (float) widget.height(), -1000.0f, 1000.0f);

    // TODO figure out center mode
    m.translate(x, y, z);
//    if (mode == CORNER) {
//        m.translate(x, y, z);
//    } else { // CENTER
//        m.translate(x - (float) texture->width() / 4.0f, y - (float) texture->height() / 4.0f, z);
//    }
//    if (mode == CENTER) { m.translate((float) texture->width() / 4.0f, (float) texture->height() / 4.0f, 0); }
    m.scale(xScale, yScale, 1.0f);
    m.rotate((float) xRot, 1.0f, 0.0f, 0.0f);
    m.rotate((float) yRot, 0.0f, 1.0f, 0.0f);
    m.rotate((float) zRot, 0.0f, 0.0f, 1.0f);
//    if (mode == CENTER) { m.translate(-(float) texture->width() / 4.0f, -(float) texture->height() / 4.0f, 0); }

    program->bind();
    program->setUniformValue("matrix", m);
    program->setUniformValue("textColor", color);

    float tempX = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        auto glyph = Fonts::getInstance().characters[font][size][*c];

        float xpos = tempX + (float) glyph.bearing.width() * xScale;
        float ypos = - (float) (glyph.size.height() - glyph.bearing.height()) * yScale;

        float w = (float) glyph.size.width() * xScale;
        float h = (float) glyph.size.height() * yScale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glyph.texture->bind();
        vbo->bind();
        vbo->write(0, vertices, sizeof(vertices));
        program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 4, 4 * sizeof(GLfloat));
        widget.glDrawArrays(GL_TRIANGLES, 0, 6);
        tempX += (float) (glyph.advance >> 6) * xScale;
    }
}
#pragma clang diagnostic pop

bool TextEntity::isFinished(GLWidget &widget) {
    return false;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
void TextEntity::setText(const std::string &text) {
    this->text = text;
}

void TextEntity::setColor(const QColor &color) {
    this->color = color;
}

#pragma clang diagnostic pop
