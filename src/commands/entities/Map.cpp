#include "commands/entities/Map.h"
#include "Fonts.h"
#include <QTimer>

const float Map::goalX = 687;
const float Map::goalY = 200.62;
const int Map::maxLaps = 2;

// Use the actual name like "map1" not ":/textures/map1.png"
// We will load both the png and bmp
Map::Map(const std::string &name, float scale) : ImageEntity(QString(":/textures/%1.png").arg(name.c_str())) {
    music.setSource(QUrl("qrc:/bgm/map1.wav"));
    music.setVolume(0.1f);
    music.setLoopCount(QSoundEffect::Infinite);

    thudSfx.setSource(QUrl("qrc:/sfx/thud.wav"));
    thudSfx.setVolume(2.5f);

    airhorn.setSource(QUrl("qrc:/sfx/airhorn.wav"));
    airhorn.setVolume(1.0f);

    mapRoute = QImage(QString(":/textures/%1.bmp").arg(name.c_str()));
    this->scale = scale;
    z = -420; // Make sure the map is always behind the vehicles
    followPerspective = true;

    this->player = std::make_shared<UserVehicle>();
    this->player->setTranslation(703, 201, 0);
    this->player->zRot = 83;
    vehicles.push_back(this->player);

//    const auto smallCalc = [](float a) {
//        return -1038.39f * a * a + 7.31337f * a;
//    };

    for (int i = 0; i < 1; i++) {
        auto cpu = std::make_shared<CPUVehicle>();
        cpu->setTranslation(555, 251, 0);
        vehicles.push_back(cpu);
    }

    startLine = std::make_shared<StartLine>();
    startLine->followPerspective = true;
    startLine->zRot = 83;
    startLine->setTranslation(goalX, goalY, -10);

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
    music.play();
}

void Map::draw(GLWidget &widget) {
    ImageEntity::draw(widget);

    std::stable_sort(vehicles.begin(), vehicles.end(), [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
        if (a->laps == b->laps) {
            return a->lastCheckpoint > b->lastCheckpoint;
        } else {
            return a->laps > b->laps;
        }
    });

    // do collision calculation
    for (int i = 0; i < vehicles.size(); ++i) {
        vehicles[i]->place = i + 1;

        for (int j = i + 1; j < vehicles.size(); ++j) {
            if (entityCollided(vehicles[i], vehicles[j])) {
                // mmmm physics (coefficient of restitution)
                auto xDir = mayhapsElasticCollision(vehicles[i]->mass, vehicles[i]->velocity.x(), vehicles[j]->mass,
                                                     vehicles[j]->velocity.x());
                auto yDir = mayhapsElasticCollision(vehicles[i]->mass, vehicles[i]->velocity.y(), vehicles[j]->mass,
                                                     vehicles[j]->velocity.y());

#define STEP_FACTOR 0.01f
                std::shared_ptr<Vehicle> vehicleBehind = vehicles[i];
                std::shared_ptr<Vehicle> vehicleAhead = vehicles[j];

                // find the vehicle that is behind - this is relative
                float firstZRot = (vehicles[i]->zRot - 90) * (float) M_PI / 180.0f;
                float secondZRot = (vehicles[j]->zRot - 90) * (float) M_PI / 180.0f;
#define DISTANCE_SCAN 500.0f
                QVector2D firstPos(vehicles[i]->x, vehicles[i]->y);
                QVector2D firstPosDist(firstPos.x() + cos(firstZRot) * DISTANCE_SCAN, firstPos.y() + sin(firstZRot) * DISTANCE_SCAN);
                QVector2D secondPos(vehicles[j]->x, vehicles[j]->y);
                QVector2D secondPosDist(secondPos.x() + cos(secondZRot) * DISTANCE_SCAN, secondPos.y() + sin(secondZRot) * DISTANCE_SCAN);
                float firstScanToSecond = (secondPos - firstPosDist).length(); // if smaller, then first is behind
                float secondScanToSecond = (firstPos - secondPosDist).length(); // if smaller, then second is behind

                if (secondScanToSecond < firstScanToSecond) {
                    vehicleBehind = vehicles[j];
                    vehicleAhead = vehicles[i];
                }

                while (entityCollided(vehicles[i], vehicles[j])) {
                    vehicleBehind->x -= vehicleBehind->velocity.x() * STEP_FACTOR;
                    vehicleBehind->y -= vehicleBehind->velocity.y() * STEP_FACTOR;
                    vehicleAhead->x += vehicleAhead->velocity.x() * STEP_FACTOR;
                    vehicleAhead->y += vehicleAhead->velocity.y() * STEP_FACTOR;
                }

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

    // check if vehicles made it to checkpoints
    for (const auto& checkpoint: checkpoints) {
        for (auto& vehicle : vehicles) {
            if (entityCollided(vehicle, checkpoint)/* && vehicle->checkpointsHit.count(checkpoint->index) == 0*/) {
//                vehicle->checkpointsHit.insert(checkpoint->index);
                if (vehicle->lastCheckpoint == checkpoint->index - 1) {
                    vehicle->lastCheckpoint = checkpoint->index;
                    this->player->data.avgSpeed += this->player->getSpeed();
                }
            }
        }
    }

    // check for vehicles who have made a lap
    for (auto& vehicle : vehicles) {
        if (entityCollided(vehicle, startLine)/* && vehicle->checkpointsHit.size() == Checkpoint::totalCheckpoints*/ && vehicle->lastCheckpoint == Checkpoint::totalCheckpoints - 1) {
//            vehicle->checkpointsHit.clear();
            vehicle->lastCheckpoint = -1;
            vehicle->laps++;

            if (vehicle.get() == this->player.get()) {
                QTime now = QTime::currentTime();
                int msecDiff = now.msec()-this->player->lapStart.msec();
                int diff = now.second()*1000+msecDiff;
                QTime timediff = QTime(diff / 1000/60/60, diff / 1000/60, diff /1000);
                this->player->data.lapTimes.push_back(timediff);
                this->player->lapStart = now;
                
            }

            if (vehicle->laps >= maxLaps) {
                if (vehicle.get() == this->player.get()) {
                    this->player->data.avgSpeed /= (((float)checkpoints.size()+2)*(maxLaps+1));
                    widget.addCommand(std::make_shared<GameStats>(this->player->data));
                }

                vehicle->effect.stop();
                vehicle->forceFinish = true;
            }
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
    // remember to stop the music
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

#define GIVE_UP_DISTANCE 1000
        if (distance > GIVE_UP_DISTANCE) {
            qDebug() << "Gave up on finding a drivable pixel for" << point;
            return point;
        }
    }
}

bool Map::isDrivable(const QPoint& point) {
    return
        point.x() >= 0 && point.y() >= 0 &&
        point.x() < mapRoute.width() && point.y() < mapRoute.height() &&
        mapRoute.pixelColor(point.x(), point.y()) == Qt::black;
}