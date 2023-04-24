#include <QMatrix4x4>
#include "commands/entities/CPUVehicle.h"

void CPUVehicle::init(GLWidget &widget) {

    Vehicle::init(widget);
}

void CPUVehicle::draw(GLWidget &widget) {

    Vehicle::draw(widget);
}

bool CPUVehicle::isFinished(GLWidget &widget) {
    return widget.isKeyPressed(Qt::Key_P);
}
