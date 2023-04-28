#ifndef CSE165PROJECT_USERVEHICLE_H
#define CSE165PROJECT_USERVEHICLE_H


#include "Vehicle.h"

class UserVehicle : public Vehicle {
public:
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
};


#endif //CSE165PROJECT_USERVEHICLE_H
