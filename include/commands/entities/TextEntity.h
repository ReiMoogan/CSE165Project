#ifndef CSE165PROJECT_TEXTENTITY_H
#define CSE165PROJECT_TEXTENTITY_H

#include <QColor>
#include "Entity.h"

class TextEntity : public Entity {
private:
    static bool programInitialized;
    static QOpenGLShaderProgram *program;
    QOpenGLBuffer* vbo;
    std::string font;
    std::string text;
    int size;
    QColor color;
public:
    TextEntity(const std::string& font, const std::string& text, int size);
    TextEntity(const std::string& font, const std::string& text, int size, float x, float y, float z);
    ~TextEntity() override;
    static void initProgram([[maybe_unused]] GLWidget& widget);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
    void setText(const std::string& text);
    void setColor(const QColor& color);
};


#endif //CSE165PROJECT_TEXTENTITY_H
