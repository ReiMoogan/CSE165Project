#include <utility>

#include "commands/entities/HUD.h"

using std::to_string;

HUD::HUD(std::shared_ptr<Map> _map) : map(std::move(_map)) {
    this->positionText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->checkpointsText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->lapsText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->speedText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->placeText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
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
    widget.addCommand(this->positionText);
    widget.addCommand(this->checkpointsText);
    widget.addCommand(this->lapsText);
    widget.addCommand(this->speedText);
    widget.addCommand(this->placeText);
}