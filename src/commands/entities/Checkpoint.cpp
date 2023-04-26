#include "commands/entities/Checkpoint.h"

Checkpoint::Checkpoint(int idx) : ImageEntity(":/textures/derp_standing.png"), index(idx) {

}

void Checkpoint::init(GLWidget &widget) {
    ImageEntity::init(widget);
}

void Checkpoint::draw(GLWidget &widget) {
    ImageEntity::draw(widget);
}