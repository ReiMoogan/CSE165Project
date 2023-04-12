#ifndef CSE165PROJECT_FONTS_H
#define CSE165PROJECT_FONTS_H

#include <unordered_map>
#include <ft2build.h>
#include <QOpenGLTexture>
#include <QVector2D>
#include FT_FREETYPE_H

#include "GLWidget.h"

// Referenced from https://learnopengl.com/In-Practice/Text-Rendering

struct Character {
    QOpenGLTexture* texture;
    QVector2D size; // Size of glyph
    QVector2D bearing; // Offset from baseline to left/top of glyph
    unsigned int advance; // Horizontal offset to advance to next glyph
};

class Fonts {
private:
    static Fonts* instance;
    FT_Library ft;
    std::unordered_map<char, FT_Face> faces;
    Fonts();
    ~Fonts();
public:
    static Fonts& getInstance() {
        static Fonts instance;
        return instance;
    }

    Fonts(Fonts const&) = delete;
    void operator=(Fonts const&) = delete;

    // font -> size -> character -> Character
    std::unordered_map<std::string, std::unordered_map<unsigned int, std::unordered_map<unsigned char, Character>>> characters;
    void loadFont(GLWidget& widget, const std::string& font, unsigned int size);
};


#endif //CSE165PROJECT_FONTS_H
