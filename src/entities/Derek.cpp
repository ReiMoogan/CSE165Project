#include "entities/Derek.h"

Derek::Derek() : ImageEntity(":/textures/derp_standing.png") {

}

void Derek::init(GLWidget &widget) {
    ImageEntity::init(widget);
}

void Derek::draw(GLWidget &widget) {
    if (widget.isKeyPressed(Qt::Key_W))
        y -= 10;
    if (widget.isKeyPressed(Qt::Key_S))
        y += 10;
    if (widget.isKeyPressed(Qt::Key_A))
        x -= 10;
    if (widget.isKeyPressed(Qt::Key_D))
        x += 10;
    if (widget.isKeyPressed(Qt::Key_Q))
        zRot -= 10;
    if (widget.isKeyPressed(Qt::Key_E))
        zRot += 10;

    ImageEntity::draw(widget);
}

bool Derek::isFinished(GLWidget &widget) {
    return widget.isKeyPressed(Qt::Key_P);
}
