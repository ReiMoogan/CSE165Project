#ifndef CSE165PROJECT_CPUVEHICLE_H
#define CSE165PROJECT_CPUVEHICLE_H

#include <random>
#include "Vehicle.h"

class CPUVehicle : public Vehicle {
public:
    static float uniformRNG();
    
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
private:

};


#endif //CSE165PROJECT_CPUVEHICLE_H
