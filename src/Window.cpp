#include <QtWidgets>

#include "GLWidget.h"
#include "Window.h"

Window::Window()
{
    // File Menu
    createMenuBar();

    // Icon
    setWindowIcon(QIcon(":/textures/logo.png"));

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

    QMenu *help = menuBar()->addMenu("&Help");
    auto *aboutGame = new QAction("About &Initial Drip", this);
    help->addAction(aboutGame);
    connect(aboutGame, &QAction::triggered, this, [this] {
        QMessageBox::about(this, tr("About Initial Drip"),
                           tr("<span style=\"font-weight:800;\">Initial Drip</span><br/>Developed by Andrew Lin and William Le for CSE-165, Spring 2023."));
    });
    auto *aboutQt = new QAction("About &Qt6", this);
    help->addAction(aboutQt);
    connect(aboutQt, &QAction::triggered, this, [this] {
        QMessageBox::aboutQt(this, tr("pain in a library"));
    });
}

void Window::createGlWidget() {
    glWidget = new GLWidget;
    glWidget->setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(glWidget);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this] {
        glWidget->update();
    });
    gameTimer->start(1000 / 60);
}

void Window::setFps(int fps) {
    // we don't have less than 1ms precision lmao
    gameTimer->setInterval(1000 / fps);
}
