#include "commands/entities/ImageButton.h"

ImageButton::ImageButton(const QString &imagePath) : ImageEntity(imagePath) {

}

ImageButton::ImageButton(const QString &imagePath, float x, float y, float z) : ImageEntity(imagePath, x, y, z) {

}

void ImageButton::init(GLWidget &widget) {
    ImageEntity::init(widget);
}

void ImageButton::draw(GLWidget &widget) {
    if (widget.isMousePressed() && !mousePreviouslyPressed) {
        mousePreviouslyPressed = true;
    } else if (!widget.isMousePressed() && mousePreviouslyPressed) {
        mousePreviouslyPressed = false;
        if (mouseOver(widget)) {
            if (onClick) {
                onClick();
            }
        }
    }

    ImageEntity::draw(widget);
}

bool ImageButton::isFinished(GLWidget &widget) {
    return isFinishedFlag;
}

bool ImageButton::mouseOver(GLWidget &widget) {
    if (mode == CORNER) {
        return widget.getMousePos().x() >= x && widget.getMousePos().x() <= x + getWidth() &&
               widget.getMousePos().y() >= y && widget.getMousePos().y() <= y + getHeight();
    } else {
        return widget.getMousePos().x() >= x - getWidth() / 2.0f && widget.getMousePos().x() <= x + getWidth() / 2.0f &&
               widget.getMousePos().y() >= y - getHeight() / 2.0f && widget.getMousePos().y() <= y + getHeight() / 2.0f;
    }
}
