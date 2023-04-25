#include "commands/MainMenu.h"
#include "commands/entities/Map.h"
#include "commands/entities/HUD.h"
#include <QApplication>

void MainMenu::init(GLWidget &widget) {
    title = std::make_shared<ImageEntity>("assets/title.png", widget.width() / 2, widget.height() / 2 - 69, 0.0f);
    startButton = std::make_shared<ImageButton>(":/textures/derp_standing.png", widget.width() / 2 - 200, widget.height() / 2 + 69, 0.0f);
    exitButton = std::make_shared<ImageButton>(":/textures/derp_standing.png", widget.width() / 2 + 200, widget.height() / 2 + 69, 0.0f);
    startButton->setScale(0.05f, 0.05f);
    exitButton->setScale(0.05f, 0.05f);
    startText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "Start", 48, widget.width() / 2 - 200, widget.height() / 2 + 69, 10.0f);
    exitText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "Exit", 48, widget.width() / 2 + 200, widget.height() / 2 + 69, 10.0f);

    startButton->onClick = [this]() {
        this->startGame = true;

        title->forceFinish = true;
        startButton->forceFinish = true;
        exitButton->forceFinish = true;
        startText->forceFinish = true;
        exitText->forceFinish = true;
    };

    exitButton->onClick = []() {
        QApplication::quit();
    };

    // widget.addCommand(title);
    widget.addCommand(startButton);
    widget.addCommand(exitButton);
    widget.addCommand(startText);
    widget.addCommand(exitText);
}

void MainMenu::draw(GLWidget &widget) {
    if (startGame && !realizedStartGame) {
        auto map = std::make_shared<Map>("map1", 1.5f);
        widget.addCommand(map);
        widget.addCommand(std::make_shared<HUD>(map));
        realizedStartGame = true;
    }
}

bool MainMenu::isFinished(GLWidget &widget) {
    return false;
}

float MainMenu::getPriority() {
    // we want this to be the first thing called (check isFinished())
    return 694201337.0f; // nice
}
