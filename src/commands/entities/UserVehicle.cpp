#include <QMatrix4x4>
#include "commands/entities/UserVehicle.h"

void UserVehicle::init(GLWidget &widget) {
    GLWidget::postPerspective = [this](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {
        matrix.translate((float) widget.width() / 2.0f - this->x, (float) widget.height() / 2.0f - this->y, 0);
    };

    GLWidget::perspective = [this](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {
        matrix.translate(-(other.getX() - this->x), -(other.getY() - this->y), 0);
        matrix.rotate(-this->zRot, 0, 0, 1);
        matrix.translate((other.getX() - this->x), (other.getY() - this->y), 0);
    };

    this->followPerspective = true;
    this->lapStart = QTime::currentTime();
    this->data.avgSpeed = 0.f;
    Vehicle::init(widget);
}

void UserVehicle::draw(GLWidget &widget) {
    if (finished) {
        Vehicle::draw(widget);
        return;
    }    

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

    this->data.maxSpeed = std::max(this->data.maxSpeed, this->getSpeed());
    Vehicle::draw(widget);
}

//bool UserVehicle::isFinished(GLWidget &widget) {
//    GLWidget::postPerspective = [](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {};
//    GLWidget::perspective = [](QMatrix4x4& matrix, GLWidget& widget, Entity& other) {};
//
//    return false;
//}
