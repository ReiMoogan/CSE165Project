#ifndef CSE165PROJECT_CPUVEHICLE_H
#define CSE165PROJECT_CPUVEHICLE_H


#include "Vehicle.h"

class CPUVehicle : public Vehicle {
public:
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
};


#endif //CSE165PROJECT_CPUVEHICLE_H
