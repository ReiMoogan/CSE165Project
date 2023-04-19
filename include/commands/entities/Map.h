#ifndef CSE165PROJECT_MAP_H
#define CSE165PROJECT_MAP_H

#include <string>
#include <vector>
#include "ImageEntity.h"
#include "Vehicle.h"
#include "UserVehicle.h"

class Map : public ImageEntity {
public:
    Map(const std::string &name, float scale);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
private:
    QImage mapRoute;
    float scale;
    std::vector<Vehicle*> vehicles;
};

#endif //CSE165PROJECT_MAP_H
