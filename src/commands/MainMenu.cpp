#include "commands/MainMenu.h"
#include "commands/entities/Map.h"
#include "commands/entities/HUD.h"
#include <QApplication>

void MainMenu::init(GLWidget &widget) {
    fps = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "FPS: unknown", 24, widget.width() - 170, 10, 100.0f);
    background = std::make_shared<ImageEntity>(":/textures/menu.png", widget.width() / 2 + 150, widget.height() / 2, -1.0f);
    title = std::make_shared<ImageEntity>(":/textures/title.png", 50, widget.height() / 2 - 300, 0.0f);
    startButton = std::make_shared<ImageButton>(":/textures/start_button.png", 50, widget.height() / 2 + 50, 0.0f);
    exitButton = std::make_shared<ImageButton>(":/textures/exit_button.png", 50, widget.height() / 2 + 200, 0.0f);
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

    music.setSource(QUrl("qrc:/bgm/menu.mp3"));
    output.setVolume(0.3f);
    music.setAudioOutput(&output);
    music.setLoops(QMediaPlayer::Infinite);
    music.play();
}

void MainMenu::draw(GLWidget &widget) {
    if (startGame && !realizedStartGame) {
        music.stop();
        this->map = std::make_shared<Map>("map1", 1.5f);
        widget.addCommand(map);
        this->hud = std::make_shared<HUD>(map, *this);
        widget.addCommand(hud);
        realizedStartGame = true;
        endGame = false;
        realizedEndGame = false;
    }

    if (endGame && !realizedEndGame) {
        widget.setClearColor(QColor(255, 255, 255));

        if (this->hud.use_count()) {
            this->hud->cleanup();
            this->hud->forceFinish = true;
            
            for (const auto& vehicle : this->map->vehicles) {
                vehicle->effect.stop();
                vehicle->forceFinish = true;
            }

            this->map->startLine->forceFinish = true;
            for (const auto& checkpoint : this->map->checkpoints) {
                checkpoint->forceFinish = true;
            }

            this->map->music.stop();
            this->map->forceFinish = true;
        }

        background->forceFinish = false;
        title->forceFinish = false;
        startButton->forceFinish = false;
        exitButton->forceFinish = false;

        widget.addCommand(background);
        widget.addCommand(title);
        widget.addCommand(startButton);
        widget.addCommand(exitButton);

        startGame = false;
        realizedStartGame = false;
        realizedEndGame = true;

        music.play();
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
