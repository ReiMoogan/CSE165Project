#include "commands/entities/HUD.h"

using std::to_string;

HUD::HUD(Map *_map) : map(_map) {
    this->positionText = new TextEntity(":/fonts/Inconsolata.ttf", "mukyu~", 48, 100, 100, 422);
}

void HUD::draw(GLWidget& widget) {
    this->positionText->setText
        ("Position: " + to_string(this->map->player->getX()) + ", " + to_string(this->map->player->getY()));
    this->positionText->draw(widget);
}

void HUD::toggleDebug() {
    this->isDebug != this->isDebug;
}

bool HUD::isFinished(GLWidget& widget) {
    return false;
}

void HUD::init(GLWidget& widget) { 
    this->positionText->setTranslation(50, widget.height()-100, 422);
    this->positionText->init(widget);
}