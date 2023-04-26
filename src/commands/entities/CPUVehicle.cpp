#include <QMatrix4x4>
#include "commands/entities/CPUVehicle.h"
#include "Checkpoint.h"

void CPUVehicle::init(GLWidget &widget) {
    Vehicle::init(widget);
}

void CPUVehicle::draw(GLWidget &widget) {
    int nextCheckpointIdx = checkpointsHit.size();
    if (nextCheckpointIdx < Checkpoint::totalCheckpoints) {
        float targetX = Checkpoint::checkpointCoords[nextCheckpointIdx][0];
        float targetY = Checkpoint::checkpointCoords[nextCheckpointIdx][0];
    }

    Vehicle::draw(widget);
}

bool CPUVehicle::isFinished(GLWidget &widget) {
    return false;
}
