#include "commands/entities/Vehicle.h"

Vehicle::Vehicle() : ImageEntity(":/textures/derp_standing.png") {

}

void Vehicle::init(GLWidget &widget) {
    setScale(0.1, 0.1);
    ImageEntity::init(widget);

    effect.setSource(QUrl("qrc:/sfx/engine.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(0.1f);
    effect.play();
}

void Vehicle::draw(GLWidget &widget) {
    switch (mode) {
        case DECELERATE:
            velocity += maxAcceleration[0] * widget.getTimeDelta();
            break;
        case ACCELERATE:
            velocity += maxAcceleration[1] * widget.getTimeDelta();
            break;
        case NONE:
            velocity += -velocity * 0.05f;
            break;
    }

    switch (turn) {
        case LEFT:
            zRot -= turnSpeed * widget.getTimeDelta();
            break;
        case RIGHT:
            zRot += turnSpeed * widget.getTimeDelta();
            break;
        case STRAIGHT:
            break;
    }

    float speed = std::abs(velocity);
    effect.setVolume(std::max(0.1f, std::min(0.9f, speed / 300.0f)));

    velocity = std::min(std::max(velocity, maxVelocity[0]), maxVelocity[1]);
    float angle = zRot - 90; // image is upright, so subtract 90 degrees (since system origin is top left)
    // oh yeah did you know QMatrix4x4 USES DEGREES INSTEAD OF RADIANS AHHHHHHHHHHHHHHHHHHHHHHHHH
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
