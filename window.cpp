#include <QtWidgets>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    QColor clearColor;
    clearColor.setHsl(69, 42, 255);

    currentGlWidget = new GLWidget;
    currentGlWidget->setClearColor(clearColor);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(currentGlWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("CSE-165 Final Project"));
}