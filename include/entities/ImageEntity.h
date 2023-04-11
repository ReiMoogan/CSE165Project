#ifndef CSE165PROJECT_IMAGEENTITY_H
#define CSE165PROJECT_IMAGEENTITY_H

#include "Entity.h"

class ImageEntity : public Entity {
private:
    static bool programInitialized;
    static QOpenGLShaderProgram *program;
    QOpenGLTexture* texture;
    QOpenGLBuffer vbo;
protected:
    float xRot = 0;
    float yRot = 0;
    float zRot = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    float xScale = 1;
    float yScale = 1;
    // no z scale
    void setTranslation(float x, float y, float z);
    void setRotation(float xRot, float yRot, float zRot);
    void setScale(float xScale, float yScale);

private:
    float getZ() override;

public:
    explicit ImageEntity(const QString& imagePath);
    ~ImageEntity() override;
    void initProgram(GLWidget& widget);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_IMAGEENTITY_H
