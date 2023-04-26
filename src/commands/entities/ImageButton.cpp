#include "commands/entities/ImageButton.h"

bool ImageButton::soundInitialized = false;
QSoundEffect ImageButton::hoverSound;
QSoundEffect ImageButton::clickSound;

ImageButton::ImageButton(const QString &imagePath) : ImageEntity(imagePath) {

}

ImageButton::ImageButton(const QString &imagePath, float x, float y, float z) : ImageEntity(imagePath, x, y, z) {

}

void ImageButton::init(GLWidget &widget) {
    if (!soundInitialized) {
        hoverSound.setSource(QUrl("qrc:/sfx/plst00.wav"));
        hoverSound.setVolume(0.1f);
        clickSound.setSource(QUrl("qrc:/sfx/click.wav"));
        clickSound.setVolume(0.4f);
        soundInitialized = true;
    }

    ImageEntity::init(widget);
}

void ImageButton::draw(GLWidget &widget) {
    bool isMouseOver = mouseOver(widget);

    if (isMouseOver) {
        select();
    } else {
        deselect();
    }

    if (widget.isMousePressed() && !mousePreviouslyPressed) {
        mousePreviouslyPressed = true;
    } else if (!widget.isMousePressed() && mousePreviouslyPressed) {
        mousePreviouslyPressed = false;
        if (isMouseOver) {
            if (onClick) {
                clickSound.play();
                onClick();
            }
        }
    }

    ImageEntity::draw(widget);
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

void ImageButton::select() {
    if (lastSelectionState)
        return;

    hoverSound.play();
    enableColorShift = true;
    hslShift = {0.35f, 0, 0};

    lastSelectionState = true;
}

void ImageButton::deselect() {
    if (!lastSelectionState)
        return;

    enableColorShift = false;
    hslShift = {0, 0, 0};

    lastSelectionState = false;
}
