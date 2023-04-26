#include <QMatrix4x4>
#include "commands/entities/CPUVehicle.h"
#include "commands/entities/Checkpoint.h"

void CPUVehicle::init(GLWidget &widget) {
    Vehicle::init(widget);
}

void CPUVehicle::draw(GLWidget &widget) {
    int nextCheckpointIdx = checkpointsHit.size();
    float targetX, targetY;
    if (nextCheckpointIdx < Checkpoint::totalCheckpoints) {
        targetX = Checkpoint::checkpointCoords[nextCheckpointIdx][0];
        targetY = Checkpoint::checkpointCoords[nextCheckpointIdx][1];
    } else {
        // targetX = 
    }

    float dX = targetX-x;
    float dY = targetY-y;

    zRot = 90+(atan2(dY, dX) * 180/M_PI) + (uniformRNG()-0.5) * 10;

    if (uniformRNG() < 0.4) {
        setAccelerator(ACCELERATE);
    } else {
        setAccelerator(NONE);
    }

    Vehicle::draw(widget);
}

float CPUVehicle::uniformRNG() {
    return rand()/(float)INT_MAX;
}

bool CPUVehicle::isFinished(GLWidget &widget) {
    return false;
}
