#include "commands/MainMenu.h"
#include "commands/entities/Map.h"
#include "commands/entities/HUD.h"
#include <QApplication>

void MainMenu::init(GLWidget &widget) {
    fps = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "FPS: unknown", 24, widget.width() - 170, 10, 100.0f);
    background = std::make_shared<ImageEntity>(":/textures/menu.png", widget.width() / 2 + 150, widget.height() / 2, -1.0f);
    title = std::make_shared<ImageEntity>(":/textures/title.png", 100, widget.height() / 2 - 300, 0.0f);
    startButton = std::make_shared<ImageButton>(":/textures/start_button.png", 100, widget.height() / 2 + 50, 0.0f);
    exitButton = std::make_shared<ImageButton>(":/textures/exit_button.png", 500, widget.height() / 2 + 50, 0.0f);
    background->setScale(1.6f, 1.6f);
    title->setScale(0.5f, 0.5f);
    title->setDrawMode(CORNER);
    startButton->setScale(0.5f, 0.5f);
    startButton->setDrawMode(CORNER);
    exitButton->setScale(0.5f, 0.5f);
    exitButton->setDrawMode(CORNER);

    startButton->onClick = [this]() {
        this->startGame = true;

        background->forceFinish = true;
        title->forceFinish = true;
        startButton->forceFinish = true;
        exitButton->forceFinish = true;
    };

    exitButton->onClick = []() {
        QApplication::quit();
    };

    widget.addCommand(fps);
}

void MainMenu::draw(GLWidget &widget) {
    if (startGame && !realizedStartGame) {
        auto map = std::make_shared<Map>("map1", 1.5f);
        widget.addCommand(map);
        widget.addCommand(std::make_shared<HUD>(map));
        realizedStartGame = true;
        endGame = false;
        realizedEndGame = false;
    }

    if (endGame && !realizedEndGame) {
        widget.setClearColor(QColor(255, 255, 255));

        widget.addCommand(background);
        widget.addCommand(title);
        widget.addCommand(startButton);
        widget.addCommand(exitButton);

        startGame = false;
        realizedStartGame = false;
        realizedEndGame = true;
    }

    fps->setText("FPS: " + std::to_string(widget.getFps()));
}

bool MainMenu::isFinished(GLWidget &widget) {
    return false;
}

float MainMenu::getPriority() {
    // we want this to be the first thing called (check isFinished())
    return 694201337.0f; // nice
}

void MainMenu::showMenuScreen() {
    endGame = true;
}
