#include "commands/entities/Map.h"

// Use the actual name like "map1" not ":/textures/map1.png"
// We will load both the png and bmp
Map::Map(const std::string &name, int scale) : ImageEntity(QString(":/textures/%1.png").arg(name.c_str())) {
    mapRoute = QImage(QString(":/textures/%1.bmp").arg(name.c_str()));
}

void Map::init(GLWidget &widget) {
    ImageEntity::init(widget);
}

void Map::draw(GLWidget &widget) {
    ImageEntity::draw(widget);
}

bool Map::isFinished(GLWidget &widget) {
    return false;
}
