#ifndef CSE165PROJECT_IMAGEENTITY_H
#define CSE165PROJECT_IMAGEENTITY_H

#include <QOpenGLVertexArrayObject>
#include "Entity.h"

class ImageEntity : public Entity {
private:
    static bool programInitialized;
    static QOpenGLShaderProgram *program;
    QOpenGLTexture** textures;
    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject* vao;
    int frames = 1;
    float delay = 1000;
    float timeSinceLastAnimatedFrame = 0;
    int frame = 0;

protected:
    bool followPerspective = false;
    // width and height of the image, after scaling
    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;

public:
    explicit ImageEntity(const QString& imagePath);
    ImageEntity(const QString& imagePath, float x, float y, float z, bool followPerspective = false);
    ~ImageEntity() override;
    static void initProgram([[maybe_unused]] GLWidget& widget);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_IMAGEENTITY_H
