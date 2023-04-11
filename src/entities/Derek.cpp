#include "entities/Derek.h"
#include <iostream>

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
    std::cout << x << ", " << y << ", " << zRot << std::endl;

    ImageEntity::draw(widget);
}

bool Derek::isFinished(GLWidget &widget) {
    return ImageEntity::isFinished(widget);
}
