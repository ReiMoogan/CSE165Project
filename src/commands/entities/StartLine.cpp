#include "commands/entities/StartLine.h"

StartLine::StartLine() : ImageEntity(":/textures/start_line.png") {

}

void StartLine::init(GLWidget &widget) {
    setScale(0.05, 0.05);
    ImageEntity::init(widget);
}

void StartLine::draw(GLWidget &widget) {
    ImageEntity::draw(widget);
}
