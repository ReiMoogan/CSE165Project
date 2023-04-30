#include "commands/entities/Vehicle.h"

Vehicle::Vehicle() : ImageEntity(":/textures/derp_convertible.png"), laps(0) {

}

void Vehicle::init(GLWidget &widget) {
    velocity = QVector2D(0, 0);
    setScale(0.1, 0.1);
    ImageEntity::init(widget);

    effect.setSource(QUrl("qrc:/sfx/engine.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(0.1f);
    effect.play();
}

void Vehicle::draw(GLWidget &widget) {
    calculateVehicle(widget);

    ImageEntity::draw(widget);
}

void Vehicle::calculateVehicle(const GLWidget &widget) {
    if (!started) return;
    
    float angle = zRot - 90; // image is upright, so subtract 90 degrees (since system origin is top left)
    // oh yeah did you know QMatrix4x4 USES DEGREES INSTEAD OF RADIANS AHHHHHHHHHHHHHHHHHHHHHHHHH

    auto direction = QVector2D(cos(angle * (float) M_PI / 180.0f), sin(angle * (float) M_PI / 180.0f));

    switch (mode) {
        case DECELERATE:
            velocity += (maxAcceleration[0] * widget.getTimeDelta()) * direction;
            break;
        case ACCELERATE:
            velocity += (maxAcceleration[1] * widget.getTimeDelta()) * direction;
            break;
        case NONE:
            velocity += -0.05f * velocity;
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

    float speed = velocity.length();
    effect.setVolume(std::max(0.1f, std::min(0.9f, speed / 300.0f)));

    float expectedSpeed = std::min(std::max(speed, maxVelocity[0]), maxVelocity[1]);
    if (speed != 0)
        velocity *= expectedSpeed / speed;

    x += velocity.x() * widget.getTimeDelta();
    y += velocity.y() * widget.getTimeDelta();
}

bool Vehicle::isFinished(GLWidget &widget) {
    return forceFinish;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

void Vehicle::setAccelerator(Acceleration mode) {
    this->mode = mode;
}

void Vehicle::setTurn(Turn turn) {
    this->turn = turn;
}

float Vehicle::getSpeed() {
    return velocity.length();
}

void Vehicle::setPaused(bool paused) {
    this->paused = paused;
    if (paused) {
        effect.stop();
    } else {
        effect.play();
    }
}

#pragma clang diagnostic pop
