#include <QDebug>
#include <QFile>
#include "Fonts.h"

Fonts::Fonts() {
    ft = nullptr;
    if (FT_Init_FreeType(&ft)) {
        qCritical() << "Failed to init FreeType";
        return;
    }
}

Fonts::~Fonts() {
    FT_Done_FreeType(ft);
}

void Fonts::loadFont(GLWidget& widget, const std::string &font, unsigned int size) {
    QFile file(font.c_str());

    if (!file.open(QFile::ReadOnly)) {
        qCritical() << "Failed to open font file";
        return;
    }

    QByteArray bytes = file.readAll();
    file.close();
    auto data = (FT_Byte*) bytes.constData();

    FT_Face face;
    if (FT_New_Memory_Face(ft, data, (FT_Long) bytes.size(), 0, &face)) {
        qCritical() << "Failed to load font";
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    // read first 128 characters, then load them into QOpenGLTextures.
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            qCritical() << "Failed to load glyph";
            continue;
        }

        widget.makeCurrent(); // Ensure we have GL context before calling OpenGL functions
        widget.glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction to avoid segfaults
        auto* texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->setFormat(QOpenGLTexture::R8_UNorm);

        if (face->glyph->bitmap.buffer != nullptr) {
            // avoid error vomiting on empty glyphs
            texture->setSize((int) face->glyph->bitmap.width, (int) face->glyph->bitmap.rows);
            texture->allocateStorage();
            texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, face->glyph->bitmap.buffer);
            texture->setWrapMode(QOpenGLTexture::ClampToEdge);
            texture->setMinificationFilter(QOpenGLTexture::Linear);
            texture->setMagnificationFilter(QOpenGLTexture::Linear);
            texture->setWrapMode(QOpenGLTexture::ClampToEdge);
        }

        Character character = {
                texture,
                // praying that these values don't go over 0x7FFFFF
                QSize((int) face->glyph->bitmap.width, (int) face->glyph->bitmap.rows),
                QSize((int) face->glyph->bitmap_left, (int) face->glyph->bitmap_top),
                face->glyph->advance.x
        };

        characters[font][size][c] = character;
    }

    FT_Done_Face(face);
}
