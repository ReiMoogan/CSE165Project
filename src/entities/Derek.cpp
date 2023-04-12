#include "entities/Derek.h"

Derek::Derek() : ImageEntity(":/textures/derp_standing.png") {

}

void Derek::init(GLWidget &widget) {
    ImageEntity::init(widget);
    setScale(0.3, 0.3);
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
    if (widget.isKeyPressed(Qt::Key_Z))
        yRot -= 2;
    if (widget.isKeyPressed(Qt::Key_C))
        yRot += 2;
    if (widget.isKeyPressed(Qt::Key_Equal)) { // + key
        xScale += 0.01;
        yScale += 0.01;
    }
    if (widget.isKeyPressed(Qt::Key_Minus)) {
        xScale -= 0.01;
        yScale -= 0.01;
    }

    GLWidget::perspective = [this](QMatrix4x4& matrix, GLWidget& widget) {
        matrix.translate((float) widget.width() / 2.0f - x, (float) widget.height() / 2.0f - y, 0);
    };

    ImageEntity::draw(widget);
}

bool Derek::isFinished(GLWidget &widget) {
    return widget.isKeyPressed(Qt::Key_P);
}
