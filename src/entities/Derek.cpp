#include <QFile>
#include "entities/Derek.h"

Derek::Derek() : ImageEntity(":/textures/derp_standing.png") {

}

void Derek::init(GLWidget &widget) {
    ImageEntity::init(widget);
    setScale(0.3, 0.3);
}

void Derek::draw(GLWidget &widget) {
    if (widget.isKeyPressed(Qt::Key_W)) {
        y -= 10*sin(zRot*M_PI/180 + M_PI/2);
        x -= 10*cos(zRot*M_PI/180 + M_PI/2);
    }
    if (widget.isKeyPressed(Qt::Key_S)) {
        y -= -10*sin(zRot*M_PI/180 + M_PI/2);
        x -= -10*cos(zRot*M_PI/180 + M_PI/2);
    }
    if (widget.isKeyPressed(Qt::Key_A))
        zRot -= 5;
    if (widget.isKeyPressed(Qt::Key_D))
        zRot += 5;
    
    GLWidget::perspective = [this](QMatrix4x4& matrix, GLWidget& widget) {
        matrix.translate((float) widget.width() / 2.0f - x, (float) widget.height() / 2.0f - y, 0);
    };

    ImageEntity::draw(widget);
}

bool Derek::isFinished(GLWidget &widget) {
    return widget.isKeyPressed(Qt::Key_P);
}
