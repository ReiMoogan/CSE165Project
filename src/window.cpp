#include <QtWidgets>

#include <glwidget.h>
#include <window.h>

Window::Window()
{
    QColor clearColor;
    clearColor.setHsl(69, 42, 255);

    currentGlWidget = new GLWidget;
    currentGlWidget->setClearColor(clearColor);
    currentGlWidget->rotateBy(+42 * 16, +42 * 16, -21 * 16);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(currentGlWidget);
    setLayout(mainLayout);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Window::renderFrame);
    timer->start(20);

    setWindowTitle(tr("CSE-165 Final Project"));
}

void Window::renderFrame()
{
    if (currentGlWidget)
        currentGlWidget->rotateBy(+2 * 16, +2 * 16, -1 * 16);
}
