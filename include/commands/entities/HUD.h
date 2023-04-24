#ifndef CSE165PROJECT_HUD_H
#define CSE165PROJECT_HUD_H

#include <QOpenGLVertexArrayObject>
#include "Entity.h"
#include "Map.h"
#include "TextEntity.h"

class HUD : public Entity {
public:
    HUD(Map *map);

    void toggleDebug();
private:
    Map *map;
    bool isDebug;

    TextEntity *positionText;

    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_HUD_H
