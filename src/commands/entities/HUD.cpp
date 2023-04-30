#include <utility>
#include <QTimer>
#include <QOpenGLShaderProgram>

#include "commands/entities/HUD.h"
#include "commands/entities/ImageButton.h"
#include "commands/entities/ImageEntity.h"

using std::to_string;

HUD::HUD(std::shared_ptr<Map> _map, MainMenu& _menu) : map(std::move(_map)), menu(_menu) {
    this->positionText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->checkpointsText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->lapsText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->speedText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->placeText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->countdownText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 128, 100, 100, 422);
}

void HUD::draw(GLWidget& widget) {
    this->positionText->setText(
        "Position: " + to_string(this->map->player->getX()) + ", " + to_string(this->map->player->getY()));
    this->checkpointsText->setText(
//        "checkpoints: " + to_string(this->map->player->checkpointsHit.size()));
        "checkpoints: " + to_string(this->map->player->lastCheckpoint + 1));
    this->lapsText->setText(
        "laps: " + to_string(this->map->player->laps));
    this->speedText->setText(
        "speed: " + to_string(this->map->player->getSpeed()));
    this->placeText->setText(
        "place: " + to_string(this->map->player->place));

    if (this->map->player->isFinished(widget)) {
        this->countdownText->setText("FINISH");
        this->exitButton->setScale(0.25, 0.25);
    }
}

void HUD::toggleDebug() {
    this->isDebug = !this->isDebug;
}

bool HUD::isFinished(GLWidget& widget) {
    return false;
}

void HUD::cleanup() {
    this->positionText->forceFinish = true;
    this->checkpointsText->forceFinish = true;
    this->lapsText->forceFinish = true;
    this->speedText->forceFinish = true;
    this->placeText->forceFinish = true;
    this->countdownText->forceFinish = true;
    this->exitButton->forceFinish = true;
}

void HUD::init(GLWidget& widget) {
    this->positionText->setTranslation(50, (float) widget.height() - 100, 422);
    this->checkpointsText->setTranslation(50, (float) widget.height() - 140, 422);
    this->lapsText->setTranslation(50, (float) widget.height() - 180, 422);
    this->speedText->setTranslation(50, (float) widget.height() - 220, 422);
    this->placeText->setTranslation(50, (float) widget.height() - 260, 422);
    this->countdownText->setTranslation((float) widget.width()/2, (float) widget.height()/2, 422);
    this->countdownText->setDrawMode(DrawMode::CENTER); 
    this->exitButton = std::make_shared<ImageButton>(":/textures/exit_button.png", widget.width() / 2 , widget.height() / 2 - 128, 0.0f);

    this->exitButton->onClick = [this]() {
        this->menu.showMenuScreen();
    };

    widget.addCommand(this->positionText);
    widget.addCommand(this->checkpointsText);
    widget.addCommand(this->lapsText);
    widget.addCommand(this->speedText);
    widget.addCommand(this->placeText);
    widget.addCommand(this->countdownText);
    widget.addCommand(this->exitButton);

    this->exitButton->setScale(0, 0);

    QTimer::singleShot(250, this, [this]() {
        this->map->thudSfx.play();
        this->countdownText->setText("3");
    });
    QTimer::singleShot(1000+250, this, [this]() {
        this->map->thudSfx.play();
        this->countdownText->setText("2");
    });
    QTimer::singleShot(2000+250, this, [this]() {
        this->map->thudSfx.play();
        this->countdownText->setText("1");
    });

    QTimer::singleShot(3000+250, this, [this, &widget]() {
        this->map->airhorn.setVolume(9.0f);
        this->map->airhorn.play();
        this->map->music.setVolume(0.5f);
        for (const auto& vehicle : this->map->vehicles) {
            vehicle->started = true;
        }
        this->countdownText->setText("GOOOOOOOO!");
        this->countdownText->setColor(QColor::fromRgb(156, 255, 156, 255));
    });
    QTimer::singleShot(4000+250, this, [this]() {
        this->countdownText->setText("");
    });
}