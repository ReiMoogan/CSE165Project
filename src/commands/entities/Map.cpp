#include "commands/entities/Map.h"

// Use the actual name like "map1" not ":/textures/map1.png"
// We will load both the png and bmp
Map::Map(const std::string &name, float scale) : ImageEntity(QString(":/textures/%1.png").arg(name.c_str())) {
    mapRoute = QImage(QString(":/textures/%1.bmp").arg(name.c_str()));
    this->scale = scale;
    z = -420; // Make sure the map is always behind the vehicles
    followPerspective = true;

    for (int i = 0; i < 7; ++i) {
        vehicles.push_back(new Vehicle());
    }

    vehicles.push_back(new UserVehicle());
}

void Map::init(GLWidget &widget) {
    setScale(scale, scale);
    setDrawMode(CORNER);
    ImageEntity::init(widget);

    for (auto vehicle : vehicles) {
        vehicle->followPerspective = true;
        widget.addCommand(vehicle);
    }
}

void Map::draw(GLWidget &widget) {
    ImageEntity::draw(widget);
}

bool Map::isFinished(GLWidget &widget) {
    return false;
}
