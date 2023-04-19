#include "commands/entities/Map.h"

// Use the actual name like "map1" not ":/textures/map1.png"
// We will load both the png and bmp
Map::Map(const std::string &name, float scale) : ImageEntity(QString(":/textures/%1.png").arg(name.c_str())) {
    mapRoute = QImage(QString(":/textures/%1.bmp").arg(name.c_str()));
    this->scale = scale;
    z = -420; // Make sure the map is always behind the vehicles
    followPerspective = true;

    for (int i = 0; i < 1; ++i) {
        auto vehicle = new Vehicle();
        vehicle->setTranslation(687, 207.62, 0);
        vehicles.push_back(vehicle);
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

    // do collision calculation
    for (int i = 0; i < vehicles.size(); ++i) {
        for (int j = i + 1; j < vehicles.size(); ++j) {
            if (vehiclesCollided(vehicles[i], vehicles[j])) {
                // i guess we can do some partially elastic collision math but i'm lazy
                qDebug("vehicles collided");
            }
        }

        // check if the vehicle is out of bounds
        float xScaled = vehicles[i]->getX() / scale;
        float yScaled = vehicles[i]->getY() / scale;
        if (vehicles[i]->getX() < 0 || vehicles[i]->getY() < 0 || xScaled > (float) mapRoute.width() || yScaled > (float) mapRoute.height()) {
            qDebug("vehicle out of bounds (like fr)");
        } else if (mapRoute.pixelColor((int) xScaled, (int) yScaled) == Qt::white) {
            qDebug("vehicle out of bounds");
        }
    }
}

bool Map::isFinished(GLWidget &widget) {
    return false;
}

bool Map::vehiclesCollided(Vehicle* a, Vehicle* b) {
    // this feels like CSE-024 again

    if (a->getX() + a->getWidth() / 2 < b->getX() - b->getWidth() / 2) return false;
    if (a->getX() - a->getWidth() / 2 > b->getX() + b->getWidth() / 2) return false;
    if (a->getY() + a->getHeight() / 2 < b->getY() - b->getHeight() / 2) return false;
    if (a->getY() - a->getHeight() / 2 > b->getY() + b->getHeight() / 2) return false;
    
    return true;
}
