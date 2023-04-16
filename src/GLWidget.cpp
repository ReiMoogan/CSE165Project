#include "GLWidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <iostream>

#include "commands/entities/ImageEntity.h"
#include "commands/entities/Derek.h"

std::function<void(QMatrix4x4& matrix, GLWidget& widget)> GLWidget::perspective = [](QMatrix4x4& matrix, GLWidget& widget) {
    // do nothing
};

GLWidget::~GLWidget()
{
    makeCurrent();
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return {50, 50};
}

QSize GLWidget::sizeHint() const
{
    return {1280, 720};
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::initializeGL()
{
    lastFrameTime = std::chrono::steady_clock::now();

    initializeOpenGLFunctions();

    qDebug() << "GL Version" << (const char*) glGetString(GL_VERSION);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    addCommand(new Derek());
    addCommand(new ImageEntity(":/textures/side1.png"));
    addCommand(new ImageEntity(":/textures/side1.png", (float) GLWidget::width(), (float) GLWidget::height(), 0));
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& [_, zEntities] : commands) {
        for (auto iter = zEntities.begin(); iter != zEntities.end(); /* no increment*/) {
            (*iter)->draw(*this);

            if ((*iter)->isFinished(*this)) {
                delete *iter;
                iter = zEntities.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    ++frames;
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> diff = std::chrono::duration_cast<std::chrono::seconds>(now - lastFrameTime);
    auto timeDiff = diff.count();
    if (timeDiff >= 1) {
        fps = (float) frames / timeDiff;
        lastFrameTime = now;
        frames = 0;
    }
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    pressedKeys.insert(event->key());
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    pressedKeys.erase(event->key());
}

void GLWidget::addCommand(Command* entity) {
    entity->init(*this);
    commands[entity->getPriority()].push_back(entity);
}

bool GLWidget::isKeyPressed(int key) const {
    return pressedKeys.contains(key);
}

void GLWidget::focusOutEvent(QFocusEvent *event) {
    pressedKeys.clear();
}

float GLWidget::getFps() const {
    return fps;
}