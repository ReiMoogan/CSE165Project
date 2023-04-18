#include "commands/entities/Vehicle.h"

Vehicle::Vehicle() : ImageEntity(":/textures/derp_standing.png") {

}

void Vehicle::init(GLWidget &widget) {
    setScale(0.1, 0.1);
    ImageEntity::init(widget);
}

void Vehicle::draw(GLWidget &widget) {
    switch (mode) {
        case DECELERATE:
            velocity -= maxAcceleration[0] * widget.getTimeDelta();
            break;
        case ACCELERATE:
            velocity += maxAcceleration[1] * widget.getTimeDelta();
            break;
        case NONE:
            velocity += -velocity * 0.05f;
            break;
    }

    velocity = std::min(std::max(velocity, maxVelocity[0]), maxVelocity[1]);
    float angle = zRot + 90; // image is upright, so add 90 degrees
    x += velocity * cos(angle * (float) M_PI / 180.0f) * widget.getTimeDelta();
    y += velocity * sin(angle * (float) M_PI / 180.0f) * widget.getTimeDelta();

    ImageEntity::draw(widget);
}

bool Vehicle::isFinished(GLWidget &widget) {
    return false;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
void Vehicle::setAccelerator(Acceleration mode) {
    this->mode = mode;
}

void Vehicle::setTurn(Turn turn) {
    this->turn = turn;
}

#pragma clang diagnostic pop
