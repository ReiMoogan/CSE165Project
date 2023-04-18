#ifndef CSE165PROJECT_MAP_H
#define CSE165PROJECT_MAP_H

#include <string>
#include "ImageEntity.h"

class Map : public ImageEntity {
public:
    Map(const std::string &name, int scale);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
private:
    QImage mapRoute;
};

#endif //CSE165PROJECT_MAP_H
