#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <set>
#include <chrono>

#include "commands/Command.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(Command)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT
public:
    using QOpenGLWidget::QOpenGLWidget;
    ~GLWidget() override;

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;
    void setClearColor(const QColor &color);
    void addCommand(Command *entity);
    [[nodiscard]] bool isKeyPressed(int key) const;
    [[nodiscard]] float getFps() const;

    static std::function<void(QMatrix4x4& matrix, GLWidget& widget)> perspective;
    friend class ImageEntity; // Allow us to call GL functions outside
    friend class TextEntity;
    friend class Fonts;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QColor clearColor = Qt::black;
    std::map<float, std::vector<Command*>> commands; // ordered by z
    std::set<int> pressedKeys;
    int frames = 0;
    float fps = 0;
    std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
};

#endif