#include <QFile>
#include "entities/Track.h"

Track::Track(Derek *_car) : ImageEntity(":/textures/bg.jpg"), car(_car) {

}

void Track::init(GLWidget &widget) {
    ImageEntity::init(widget);
    setScale(0.3, 0.3);
}

void Track::draw(GLWidget &widget) {
    GLWidget::perspective = [this](QMatrix4x4& matrix, GLWidget& widget) {
        matrix.translate((float) widget.width() / 2.0f - car->game_x, (float) widget.height() / 2.0f - car->game_y, 0);
        matrix.scale(20.0);
    };

    ImageEntity::draw(widget);
}

bool Track::isFinished(GLWidget &widget) {
    return widget.isKeyPressed(Qt::Key_P);
}
