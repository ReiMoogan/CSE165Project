#ifndef CSE165PROJECT_VEHICLE_H
#define CSE165PROJECT_VEHICLE_H

#include "ImageEntity.h"

enum Acceleration {
    DECELERATE,
    ACCELERATE,
    NONE
};

enum Turn {
    LEFT,
    RIGHT,
    STRAIGHT
};

class Vehicle : public ImageEntity {
public:
    Vehicle();
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
protected:
    // "tuples" of min and max values for velocity and acceleration
    float maxVelocity[2] = { -100, 500 };
    // set acceleration directly (assignment), velocity computed wrt timeDelta
    float maxAcceleration[2] = { -50, 250 };
    // turn speed left and right
    float turnSpeed = 100;

    // computed in draw(), do not directly set
    float velocity = 0;

    // configurable acceleration and turn based on user input or AI
    Acceleration mode = NONE;
    Turn turn = STRAIGHT;
    void setAccelerator(Acceleration mode);
    void setTurn(Turn turn);
};


#endif //CSE165PROJECT_VEHICLE_H