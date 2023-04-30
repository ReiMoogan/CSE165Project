#ifndef CSE165PROJECT_MAP_H
#define CSE165PROJECT_MAP_H

#include <string>
#include <vector>
#include <QSoundEffect>
#include "ImageEntity.h"
#include "Vehicle.h"
#include "CPUVehicle.h"
#include "UserVehicle.h"
#include "StartLine.h"
#include "Checkpoint.h"
#include "commands/MainMenu.h"

class Map : public ImageEntity {
public:
    Map(const std::string &name, float scale);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;

    static const float goalX, goalY;
    static const int maxLaps;
    friend class HUD;
    friend class MainMenu;
private:
    QSoundEffect music;
    QSoundEffect thudSfx;
    QSoundEffect airhorn;
    QImage mapRoute;
    float scale;
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::shared_ptr<StartLine> startLine;
    std::shared_ptr<UserVehicle> player;
    
    // checkpoint locations
    std::vector<std::shared_ptr<Checkpoint>> checkpoints;

    static bool entityCollided(const std::shared_ptr<ImageEntity>& a, const std::shared_ptr<ImageEntity>& b);
    bool isDrivable(const QPoint& point);
    QPoint getClosestDrivablePixel(const QPoint& point);
    void tpToClosestDrivablePixel(int i, float xScaled, float yScaled);
    [[nodiscard]] static std::pair<float, float> mayhapsElasticCollision(float m1, float v1, float m2, float v2);
};

#endif //CSE165PROJECT_MAP_H
