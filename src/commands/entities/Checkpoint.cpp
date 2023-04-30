#include "commands/entities/Checkpoint.h"
#include <QOpenGLShaderProgram>

const float Checkpoint::checkpointCoords[][2] = {{1078, 106}, {1650, 184}, {2099, 219}, {1915, 744}, {2250, 798}, {2531, 381}, {2770, 505}, {2508, 994}, 
    {1891, 1160}, {840, 1442}, {340, 1314}, {133, 850}, {290, 418}};
const int Checkpoint::totalCheckpoints = sizeof(Checkpoint::checkpointCoords)/sizeof(Checkpoint::checkpointCoords[0]);

Checkpoint::Checkpoint(int idx) : ImageEntity(":/textures/side1.png"), index(idx) {

}

void Checkpoint::init(GLWidget &widget) {
    ImageEntity::init(widget);
}

void Checkpoint::draw(GLWidget &widget) {
    ImageEntity::program->bind();
    // ImageEntity::draw(widget);
}