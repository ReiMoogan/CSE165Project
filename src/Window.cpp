#include <QtWidgets>

#include "GLWidget.h"
#include "Window.h"

Window::Window()
{
    QColor clearColor;
    clearColor.setHsl(69, 42, 255);

    currentGlWidget = new GLWidget;
    currentGlWidget->setClearColor(clearColor);
    currentGlWidget->setFocusPolicy(Qt::StrongFocus);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(currentGlWidget);
    setLayout(mainLayout);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] { currentGlWidget->update(); });
    timer->start(16);

    setWindowTitle(tr("CSE-165 Final Project"));
}