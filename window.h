#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void renderFrame();

private:
    GLWidget *currentGlWidget;
};

#endif
