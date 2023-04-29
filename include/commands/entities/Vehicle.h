#ifndef CSE165PROJECT_VEHICLE_H
#define CSE165PROJECT_VEHICLE_H

#include <QSoundEffect>
#include <QVector2D>
#include <unordered_set>
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
    void setFinished(GLWidget& widget, bool finished);
    float getSpeed();
    void setPaused(bool paused);
    QSoundEffect effect;

    // checkpoints
//    std::unordered_set<int> checkpointsHit;
    int place = 1;
    int lastCheckpoint = -1;
    int laps;
    bool started = false;
    friend class Map;
protected:
    bool finished = false;

    // "tuples" of min and max values for velocity and acceleration
    float maxVelocity[2] = { -100, 500 };
    // set acceleration directly (assignment), velocity computed wrt timeDelta
    float maxAcceleration[2] = { -3000, 3000 };
    // turn speed left and right
    float turnSpeed = 200;
    // mass (for collision calculations), remember to also change acceleration/velocity
    // units? good question
    float mass = 20;
    // computed in draw(), do not directly set (unless you know what you're doing)
    QVector2D velocity{};

    // configurable acceleration and turn based on user input or AI
    Acceleration mode = NONE;
    Turn turn = STRAIGHT;
    void setAccelerator(Acceleration mode);
    void setTurn(Turn turn);

    // sfx
private:
    bool paused;
    void calculateVehicle(const GLWidget &widget);
};


#endif //CSE165PROJECT_VEHICLE_H
