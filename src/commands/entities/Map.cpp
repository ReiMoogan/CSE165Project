#include "commands/entities/Map.h"
#include "Fonts.h"

const float Map::goalX = 687;
const float Map::goalY = 200.62;

// Use the actual name like "map1" not ":/textures/map1.png"
// We will load both the png and bmp
Map::Map(const std::string &name, float scale) : ImageEntity(QString(":/textures/%1.png").arg(name.c_str())) {
    mapRoute = QImage(QString(":/textures/%1.bmp").arg(name.c_str()));
    this->scale = scale;
    z = -420; // Make sure the map is always behind the vehicles
    followPerspective = true;

    // for (int i = 0; i < 1; ++i) {
    //     auto vehicle = std::make_shared<Vehicle>();
    //     vehicle->setTranslation(687, 207.62, 0);
    //     vehicles.push_back(vehicle);
    // }

    this->player = std::make_shared<UserVehicle>();
    this->player->setTranslation(703, 201, 0);
    this->player->zRot = 83;
    vehicles.push_back(this->player);

    auto cpu = std::make_shared<CPUVehicle>();
    cpu->setTranslation(555, 251, 0);
    vehicles.push_back(cpu);

    startLine = std::make_shared<StartLine>();
    startLine->followPerspective = true;
    startLine->zRot = 83;
    startLine->setTranslation(goalX, goalY, 0);

    for (int i = 0; i < 14; i++) {
        auto checkpoint = std::make_shared<Checkpoint>(i);
        checkpoint->setScale(0.6, 0.6);
        checkpoint->setTranslation(Checkpoint::checkpointCoords[i][0], Checkpoint::checkpointCoords[i][1], 0);
        checkpoints.push_back(checkpoint);
    }
}

void Map::init(GLWidget &widget) {
    widget.setClearColor(QColor(82, 156, 82));
    
    setScale(scale, scale);
    setDrawMode(CORNER);
    ImageEntity::init(widget);

    for (const auto& vehicle : vehicles) {
        vehicle->followPerspective = true;
        widget.addCommand(vehicle);
    }

    for (const auto& checkpoint : checkpoints) {
        checkpoint->followPerspective = true;
        widget.addCommand(checkpoint);
    }

    widget.addCommand(this->startLine);
}

void Map::draw(GLWidget &widget) {
    ImageEntity::draw(widget);

    // do collision calculation
    for (int i = 0; i < vehicles.size(); ++i) {
        for (int j = i + 1; j < vehicles.size(); ++j) {
            if (entityCollided(vehicles[i], vehicles[j])) {
                // mmmm physics (coefficient of restitution)
                // auto xDir = mayhapsElasticCollision(vehicles[i]->mass, vehicles[i]->velocity.x(), vehicles[j]->mass,
                                                    // vehicles[j]->velocity.x());
                // auto yDir = mayhapsElasticCollision(vehicles[i]->mass, vehicles[i]->velocity.y(), vehicles[j]->mass,
                                                    // vehicles[j]->velocity.y());
                
                float firstXDir = vehicles[i]->velocity.x();
                float firstYDir = vehicles[i]->velocity.y();
                float firstNormalizer = 1/sqrt(firstXDir*firstXDir + firstYDir*firstYDir + 0.001);
                
                float secondXDir = vehicles[j]->velocity.x();
                float secondYDir = vehicles[j]->velocity.y();
                float secondNormalizer = 1/sqrt(secondXDir*secondXDir + secondYDir*secondYDir + 0.001);

                vehicles[i]->x -= firstNormalizer*10.*firstXDir;
                vehicles[i]->y -= firstNormalizer*10.*firstYDir;
                vehicles[j]->x -= secondNormalizer*10.*secondXDir;
                vehicles[j]->y -= secondNormalizer*10.*secondYDir;

                // vehicles[i]->velocity.setX(xDir.first);
                // vehicles[i]->velocity.setY(yDir.first);
                // vehicles[j]->velocity.setX(xDir.second);
                // vehicles[j]->velocity.setY(yDir.second);
            }
        }

        // check if the vehicle is out of bounds
        float xScaled = vehicles[i]->getX() / scale;
        float yScaled = vehicles[i]->getY() / scale;
        if (!isDrivable(QPoint((int) xScaled, (int) yScaled))) {
            tpToClosestDrivablePixel(i, xScaled, yScaled);
        }
    }

    // check if vehicles made it to checkpoints
    for (const auto& checkpoint: checkpoints) {
        for (auto& vehicle : vehicles) {
            if (entityCollided(vehicle, checkpoint) && vehicle->checkpointsHit.count(checkpoint->index) == 0) {
                vehicle->checkpointsHit.insert(checkpoint->index);
            }
        }
    }

    // check for vehicles who have made a lap
    for (auto& vehicle : vehicles) {
        if (entityCollided(vehicle, startLine) && vehicle->checkpointsHit.size() == Checkpoint::totalCheckpoints) {
            vehicle->checkpointsHit.clear();
            vehicle->laps++;
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

bool Map::entityCollided(const std::shared_ptr<ImageEntity>& a, const std::shared_ptr<ImageEntity>& b) {
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