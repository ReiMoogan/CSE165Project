#ifndef CSE165PROJECT_HUD_H
#define CSE165PROJECT_HUD_H

#include <QOpenGLVertexArrayObject>
#include "Entity.h"
#include "Map.h"
#include "TextEntity.h"

class HUD : public Entity {
public:
    explicit HUD(std::shared_ptr<Map> map);

    void toggleDebug();
private:
    std::shared_ptr<Map> map;
    bool isDebug = false;

    std::shared_ptr<TextEntity> positionText;
    std::shared_ptr<TextEntity> checkpointsText;
    std::shared_ptr<TextEntity> lapsText;

    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_HUD_H
