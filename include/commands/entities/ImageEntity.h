#ifndef CSE165PROJECT_IMAGEENTITY_H
#define CSE165PROJECT_IMAGEENTITY_H

#include <QOpenGLVertexArrayObject>
#include "Entity.h"

class ImageEntity : public Entity {
private:
    static bool programInitialized;
    static QOpenGLShaderProgram *program;
    QOpenGLTexture* texture;
    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject* vao;

protected:
    bool followPerspective = false;

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
