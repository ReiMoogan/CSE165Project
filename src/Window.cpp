#include <QtWidgets>

#include "GLWidget.h"
#include "Window.h"

Window::Window()
{
    // File Menu
    createMenuBar();

    // GL Setup
    createGlWidget();

    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("CSE-165 Final Project"));
}

void Window::createMenuBar() {
    auto *quit = new QAction("&Quit", this);
    QMenu *game = menuBar()->addMenu("&Game");
    game->addAction(quit);
    connect(quit, &QAction::triggered, this, QApplication::quit);

    const int FPS_SETTINGS[4] = {30, 60, 100, 120};
    QMenu *fpsMenu = menuBar()->addMenu("&FPS");
    for (auto& fps : FPS_SETTINGS) {
        auto *action = new QAction(QString::number(fps) + " FPS", this);
        fpsMenu->addAction(action);
        connect(action, &QAction::triggered, this, [this, fps] { setFps(fps); });
    }

    fpsDisplay = menuBar()->addMenu("Current FPS: 0");
    fpsDisplay->setDisabled(true);
}

void Window::createGlWidget() {
    glWidget = new GLWidget;
    glWidget->setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(glWidget);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this] {
        glWidget->update();
        fpsDisplay->setTitle("Current FPS: " + QString::number(glWidget->getFps()));
    });
    gameTimer->start(1000 / 60);
}

void Window::setFps(int fps) {
    // we don't have less than 1ms precision lmao
    gameTimer->setInterval(1000 / fps);
}
