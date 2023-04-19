#include <QMatrix4x4>
#include "commands/entities/UserVehicle.h"

void UserVehicle::init(GLWidget &widget) {
    GLWidget::postPerspective = [this](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {
        matrix.translate((float) widget.width() / 2.0f - this->x, (float) widget.height() / 2.0f - this->y, 0);
    };

    GLWidget::perspective = [this](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {
        //matrix.translate(-(other.getX() - this->x), -(other.getY() - this->y), 0);
        matrix.rotate(-this->zRot, 0, 0, 1);
        //matrix.translate((other.getX() - this->x), (other.getY() - this->y), 0);
    };

    this->followPerspective = true;
    Vehicle::init(widget);
}

void UserVehicle::draw(GLWidget &widget) {
    if (widget.isKeyPressed(Qt::Key_W) && widget.isKeyPressed(Qt::Key_S))
        setAccelerator(NONE); // NOLINT(bugprone-branch-clone)
    else if (widget.isKeyPressed(Qt::Key_W))
        setAccelerator(ACCELERATE);
    else if (widget.isKeyPressed(Qt::Key_S))
        setAccelerator(DECELERATE);
    else
        setAccelerator(NONE);

    if (widget.isKeyPressed(Qt::Key_A) && widget.isKeyPressed(Qt::Key_D))
        setTurn(STRAIGHT); // NOLINT(bugprone-branch-clone)
    else if (widget.isKeyPressed(Qt::Key_A))
        setTurn(LEFT);
    else if (widget.isKeyPressed(Qt::Key_D))
        setTurn(RIGHT);
    else
        setTurn(STRAIGHT);

    if (widget.isKeyPressed(Qt::Key_Equal)) { // + key
        xScale += 0.01;
        yScale += 0.01;
    }
    if (widget.isKeyPressed(Qt::Key_Minus)) {
        xScale -= 0.01;
        yScale -= 0.01;
    }

    Vehicle::draw(widget);
}

bool UserVehicle::isFinished(GLWidget &widget) {
    return widget.isKeyPressed(Qt::Key_P);
}
