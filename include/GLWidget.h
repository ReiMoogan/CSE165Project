#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <set>

#include "entities/Entity.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(Entity)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    ~GLWidget() override;

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;
    void setClearColor(const QColor &color);
    void addEntity(Entity* entity);
    bool isKeyPressed(int key) const;

    friend class ImageEntity; // Allow us to call GL functions outside

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QColor clearColor = Qt::black;
    std::map<float, std::vector<Entity*>> entities; // ordered by z
    std::set<int> pressedKeys;
    int frames = 0;
    float fps = 0;
    clock_t lastFrameTime;
};

#endif