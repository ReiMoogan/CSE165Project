#include <QMatrix4x4>
#include "commands/entities/CPUVehicle.h"
#include "commands/entities/Checkpoint.h"
#include "commands/entities/Map.h"

void CPUVehicle::init(GLWidget &widget) {
    Vehicle::init(widget);
}

void CPUVehicle::draw(GLWidget &widget) {
    //auto nextCheckpointIdx = checkpointsHit.size();
    auto nextCheckpointIdx = lastCheckpoint + 1;
    float targetX, targetY;
    if (nextCheckpointIdx < Checkpoint::totalCheckpoints) {
        targetX = Checkpoint::checkpointCoords[nextCheckpointIdx][0];
        targetY = Checkpoint::checkpointCoords[nextCheckpointIdx][1];
    } else {
        targetX = Map::goalX;
        targetY = Map::goalY;
    }

    // Add some randomness to the target
    targetX += uniformRNG() * 5 - 2.5f;
    targetY += uniformRNG() * 5 - 2.5f;

    float dX = targetX-x;
    float dY = targetY-y;

    float targetZRot = 90.0f + (atan2(dY, dX) * 180.0f / (float) M_PI);
    float dZRot = targetZRot - zRot;

    if (dZRot < -1.0f || dZRot > 1.0f) {
        zRot += 0.1f * dZRot;
    }

    if (uniformRNG() < 0.4) {
        setAccelerator(ACCELERATE);
    } else {
        setAccelerator(NONE);
    }

    Vehicle::draw(widget);
}

float CPUVehicle::uniformRNG() {
    return (float) rand() / (float)RAND_MAX;
}

bool CPUVehicle::isFinished(GLWidget &widget) {
    return false;
}
