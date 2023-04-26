#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class GLWidget;

class Window : public QMainWindow
{
Q_OBJECT

public:
    Window();

private:
    GLWidget *glWidget;
    QTimer* gameTimer;

    void createMenuBar();
    void createGlWidget();
    void setFps(int fps);
};

#endif