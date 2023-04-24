#include "commands/entities/ImageButton.h"

void ImageButton::init(GLWidget &widget) {
    ImageEntity::init(widget);
}

void ImageButton::draw(GLWidget &widget) {
    ImageEntity::draw(widget);
}

bool ImageButton::isFinished(GLWidget &widget) {
    return isFinishedFlag;
}
