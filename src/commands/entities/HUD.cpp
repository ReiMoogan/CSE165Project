#include <utility>

#include "commands/entities/HUD.h"

using std::to_string;

HUD::HUD(std::shared_ptr<Map> _map) : map(std::move(_map)) {
    this->positionText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
    this->checkpointsText = std::make_shared<TextEntity>(":/fonts/Inconsolata.ttf", "", 48, 100, 100, 422);
}

void HUD::draw(GLWidget& widget) {
    this->positionText->setText(
        "Position: " + to_string(this->map->player->getX()) + ", " + to_string(this->map->player->getY()));
    this->checkpointsText->setText(
        "checkpoints: " + to_string(this->map->player->checkpointsHit.size()));
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
    widget.addCommand(this->positionText);
    widget.addCommand(this->checkpointsText);
}