#include <utility>
#include <QTimer>

#include "commands/entities/HUD.h"

using std::to_string;

HUD::HUD(std::shared_ptr<Map> _map) : map(std::move(_map)) {
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
    }
}

void HUD::toggleDebug() {
    this->isDebug = !this->isDebug;
}

bool HUD::isFinished(GLWidget& widget) {
    return false;
}

void HUD::init(GLWidget& widget) {
    this->positionText->setTranslation(50, (float) widget.height() - 100, 422);
    this->checkpointsText->setTranslation(50, (float) widget.height() - 140, 422);
    this->lapsText->setTranslation(50, (float) widget.height() - 180, 422);
    this->speedText->setTranslation(50, (float) widget.height() - 220, 422);
    this->placeText->setTranslation(50, (float) widget.height() - 260, 422);
    this->countdownText->setTranslation((float) widget.width()/2, (float) widget.height()/2, 422);
    this->countdownText->setDrawMode(DrawMode::CENTER); 
    widget.addCommand(this->positionText);
    widget.addCommand(this->checkpointsText);
    widget.addCommand(this->lapsText);
    widget.addCommand(this->speedText);
    widget.addCommand(this->placeText);
    widget.addCommand(this->countdownText);

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
        this->map->thudSfx.setVolume(3.0f);
        this->map->thudSfx.play();
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