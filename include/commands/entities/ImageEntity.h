#ifndef CSE165PROJECT_IMAGEENTITY_H
#define CSE165PROJECT_IMAGEENTITY_H

#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include "Entity.h"

class ImageEntity : public Entity {
private:
    static bool programInitialized;
    QOpenGLTexture** textures;
    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject* vao;
    int frames = 1;
    float delay = 1000;
    float timeSinceLastAnimatedFrame = 0;
    int frame = 0;

protected:
    static QOpenGLShaderProgram *program;

public:
    explicit ImageEntity(const QString& imagePath);
    ImageEntity(const QString& imagePath, float x, float y, float z, bool followPerspective = false);
    ~ImageEntity() override;
    static void initProgram([[maybe_unused]] GLWidget& widget);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
    // width and height of the image, after scaling
    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;
    bool followPerspective = false;
    // on-the-fly color filter
    bool enableColorShift = false;
    QVector3D hslShift = {0, 0, 0};
};


#endif //CSE165PROJECT_IMAGEENTITY_H
