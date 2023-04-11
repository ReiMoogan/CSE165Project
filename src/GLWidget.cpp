#include "GLWidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

#include "entities/ImageEntity.h"

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
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    addEntity(new ImageEntity(":/textures/side1.png"));
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& [_, entities] : entities) {
        for (auto iter = entities.begin(); iter != entities.end(); /* no increment*/) {
            (*iter)->draw(*this);

            if ((*iter)->isFinished(*this)) {
                delete *iter;
                iter = entities.erase(iter);
            } else {
                ++iter;
            }
        }
    }
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            // move forward
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            // move backward
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            // move left
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            // move right
            break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    // lol idk
}

void GLWidget::addEntity(Entity *entity) {
    entity->init(*this);
    entities[entity->getZ()].push_back(entity);
}
