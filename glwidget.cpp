#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    delete texture;
    delete program;
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

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/image.vert");
    qDebug() << "Compiling image vertex shader" << program->log();
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/image.frag");
    qDebug() << "Compiling image fragment shader" << program->log();
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    qDebug() << "Linking image shader" << program->log();

    program->bind();
    program->setUniformValue("texture", 0);
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.scale(1.0f / (float) GLWidget::width(), 1.0f / (float) GLWidget::height());
    m.ortho(-1.0f, +1.0f, +1.0f, -1.0f, -1000.0f, 1000.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate((float) xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate((float) yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate((float) zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->position().toPoint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->position().toPoint().x() - lastPos.x();
    int dy = event->position().toPoint().y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->position().toPoint();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit onClick();
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

void GLWidget::makeObject()
{
    static const float coord[4][3] = {
        // bottom right, bottom left, top left, top right (CCW order)
        { +1, -1, 0 }, { -1, -1, 0 }, { -1, +1, 0 }, { +1, +1, 0 }
    };

    texture = new QOpenGLTexture(QImage(QString(":/textures/side1.png")).mirrored());

    QList<GLfloat> vertData;
    for (int j = 0; j < 4; ++j) {
        // vertex position
        vertData.append(100.0f * coord[j][0]);
        vertData.append(100.0f * coord[j][1]);
        vertData.append(100.0f * coord[j][2]);
        // texture coordinate
        vertData.append(j == 0 || j == 3);
        vertData.append(j == 0 || j == 1);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), (int) (vertData.count() * sizeof(GLfloat)));
}
