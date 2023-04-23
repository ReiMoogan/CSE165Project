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
                // mmmm physics (coefficient of restitution)
                const float cr = 0.8;
                auto xDir = mayhapsElasticCollision(vehicles[i]->mass, vehicles[i]->velocity.x(), vehicles[j]->mass,
                                                    vehicles[j]->velocity.x());
                auto yDir = mayhapsElasticCollision(vehicles[i]->mass, vehicles[i]->velocity.y(), vehicles[j]->mass,
                                                    vehicles[j]->velocity.y());
                vehicles[i]->velocity.setX(xDir.first);
                vehicles[i]->velocity.setY(yDir.first);
                vehicles[j]->velocity.setX(xDir.second);
                vehicles[j]->velocity.setY(yDir.second);
            }
        }

        // check if the vehicle is out of bounds
        float xScaled = vehicles[i]->getX() / scale;
        float yScaled = vehicles[i]->getY() / scale;
        if (!isDrivable(QPoint((int) xScaled, (int) yScaled))) {
            tpToClosestDrivablePixel(i, xScaled, yScaled);
        }
    }
}

std::pair<float, float> Map::mayhapsElasticCollision(float m1, float v1, float m2, float v2) {
    float commonNumerator = m1 * v1 + m2 * v2;
    float commonDenominator = m1 + m2;

#define COEFFICIENT_OF_RESTITUTION 0.8f
    float va = (commonNumerator + m2 * COEFFICIENT_OF_RESTITUTION * (v2 - v1)) / commonDenominator;
    float vb = (commonNumerator + m1 * COEFFICIENT_OF_RESTITUTION * (v1 - v2)) / commonDenominator;

    return { va, vb };
}

void Map::tpToClosestDrivablePixel(int i, float xScaled, float yScaled) {
    auto point = getClosestDrivablePixel(QPoint((int) xScaled, (int) yScaled));
    vehicles[i]->setTranslation((float) point.x() * scale, (float) point.y() * scale, 0);
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

QPoint Map::getClosestDrivablePixel(const QPoint& point) {
    // taxicab distance lol
    int distance = 0;
    while (true) {
        // top and bottom row
        for (int x = point.x() - distance; x <= point.x() + distance; ++x) {
            if (isDrivable(QPoint(x, point.y() - distance))) return {x, point.y() - distance};
            if (isDrivable(QPoint(x, point.y() + distance))) return {x, point.y() + distance};
        }
        // left and right columns
        for (int y = point.y() - distance; y <= point.y() + distance; ++y) {
            if (isDrivable(QPoint(point.x() - distance, y))) return {point.x() - distance, y};
            if (isDrivable(QPoint(point.x() + distance, y))) return {point.x() + distance, y};
        }

        ++distance;
    }
}

bool Map::isDrivable(const QPoint& point) {
    return
        point.x() >= 0 && point.y() >= 0 &&
        point.x() < mapRoute.width() && point.y() < mapRoute.height() &&
        mapRoute.pixelColor(point.x(), point.y()) == Qt::black;
}