#ifndef CSE165PROJECT_USERVEHICLE_H
#define CSE165PROJECT_USERVEHICLE_H


#include <QTime>
#include "Vehicle.h"
#include "commands/GameStats.h"

class UserVehicle : public Vehicle {
public:
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;

    QTime lapStart;
    int avgSpeedCounter = 0;
    StatData data;
};


#endif //CSE165PROJECT_USERVEHICLE_H
