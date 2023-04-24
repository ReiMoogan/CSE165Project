#ifndef CSE165PROJECT_MAP_H
#define CSE165PROJECT_MAP_H

#include <string>
#include <vector>
#include "ImageEntity.h"
#include "Vehicle.h"
#include "CPUVehicle.h"
#include "UserVehicle.h"
#include "StartLine.h"

class Map : public ImageEntity {
public:
    Map(const std::string &name, float scale);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;

    friend class HUD;
private:
    QImage mapRoute;
    float scale;
    std::vector<Vehicle*> vehicles;
    StartLine* startLine;
    UserVehicle *player;

    static bool vehiclesCollided(Vehicle* a, Vehicle* b);
    bool isDrivable(const QPoint& point);
    QPoint getClosestDrivablePixel(const QPoint& point);
    void tpToClosestDrivablePixel(int i, float xScaled, float yScaled);
    [[nodiscard]] static std::pair<float, float> mayhapsElasticCollision(float m1, float v1, float m2, float v2);
};

#endif //CSE165PROJECT_MAP_H
