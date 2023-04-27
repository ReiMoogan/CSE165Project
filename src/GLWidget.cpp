#include "GLWidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <iostream>
#include <QOpenGLDebugLogger>
#include <ranges>

#include "commands/MainMenu.h"
#include "commands/GameStats.h"

std::function<void(QMatrix4x4& matrix, GLWidget& widget, Entity& other)> GLWidget::postPerspective = [](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {
    // do nothing
};

std::function<void(QMatrix4x4& matrix, GLWidget& widget, Entity& other)> GLWidget::perspective = [](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {
    // do nothing
};

GLWidget::~GLWidget()
{
    makeCurrent();
    // Teardown GL objects, passively invoke destructors
    commands.clear();
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
    auto *logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, [](const QOpenGLDebugMessage &msg) {
        qDebug() << msg;
    });
    logger->startLogging();
    qDebug() << "OpenGL Logging enabled:" << context()->hasExtension(QByteArrayLiteral("GL_KHR_debug"));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);

    setMouseTracking(true);

    addCommand(std::make_shared<MainMenu>());
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw from back to front
    //for (auto & command : std::ranges::reverse_view(commands)) {
    for (auto & command : commands) {
        auto& zEntities = command.second;
        for (auto iter = zEntities.begin(); iter != zEntities.end(); /* no increment*/) {
            (*iter)->draw(*this);

            std::shared_ptr<Command> temp = *iter;
            bool isFinished = (temp)->isFinished(*this);
            isFinished |= (*iter)->forceFinish;

            if (isFinished) {
                iter = zEntities.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    ++frames;
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime);
    timeDelta = diff.count(); // compute time diff between frames in seconds
    lastFrameTime = now;
    fpsTime += timeDelta; // fps timer only resets every second

    if (fpsTime >= 1) {
        fps = (float) frames / fpsTime;
        fpsTime = 0;
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

void GLWidget::addCommand(const std::shared_ptr<Command>& entity) {
    entity->init(*this);
    commands[entity->getPriority()].push_back(entity);
}

bool GLWidget::isKeyPressed(int key) const {
    return pressedKeys.contains(key);
}

void GLWidget::focusOutEvent(QFocusEvent *event) {
    pressedKeys.clear();
    mousePressed = false;
    mousePos.setX(-69420); // prevent mouse from being processed when window is not focused
    mousePos.setY(-1337);
}

float GLWidget::getFps() const {
    return fps;
}

float GLWidget::getTimeDelta() const {
    return timeDelta;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    mousePressed = true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    mousePressed = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mousePos = event->position();
}

QPointF GLWidget::getMousePos() const {
    return mousePos;
}

bool GLWidget::isMousePressed() const {
    return mousePressed;
}
