#include "GLWidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

#include "entities/ImageEntity.h"
#include "entities/Derek.h"

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
    addEntity(new Derek());
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
    pressedKeys.insert(event->key());
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    pressedKeys.erase(event->key());
}

void GLWidget::addEntity(Entity *entity) {
    entity->init(*this);
    entities[entity->getZ()].push_back(entity);
}

bool GLWidget::isKeyPressed(int key) const {
    return pressedKeys.contains(key);
}
