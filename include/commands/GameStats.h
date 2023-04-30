#ifndef CSE165PROJECT_GAMESTATS_H
#define CSE165PROJECT_GAMESTATS_H

#include <string>
#include <QTime>
#include "GLWidget.h"

struct StatData {
    std::string place;
    std::vector<QTime> lapTimes;
    float maxSpeed;
    float avgSpeed;
};

class GameStats : public Command {
public:
    explicit GameStats(const StatData& data);
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
private:
    bool boxClosed = false;
    std::string message;
};


#endif //CSE165PROJECT_GAMESTATS_H
