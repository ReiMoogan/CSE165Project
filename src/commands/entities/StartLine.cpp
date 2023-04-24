#include "commands/entities/StartLine.h"

StartLine::StartLine() : ImageEntity(":/textures/start_line.png") {
    followPerspective = true;
}

void StartLine::init(GLWidget &widget) {
    setScale(0.3, 0.3);
    ImageEntity::init(widget);
}

void StartLine::draw(GLWidget &widget) {
    ImageEntity::draw(widget);
}
