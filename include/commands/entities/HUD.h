#ifndef CSE165PROJECT_HUD_H
#define CSE165PROJECT_HUD_H

#include <QOpenGLVertexArrayObject>
#include "Entity.h"
#include "Map.h"
#include "TextEntity.h"
#include "commands/entities/ImageButton.h"
#include "commands/MainMenu.h"

class MainMenu;
class Map;

class HUD : public Entity {
public:
    explicit HUD(std::shared_ptr<Map> map, MainMenu& menu);

    void toggleDebug();
    void cleanup();
private:
    std::shared_ptr<Map> map;
    bool isDebug = false;

    std::shared_ptr<TextEntity> positionText;
    std::shared_ptr<TextEntity> checkpointsText;
    std::shared_ptr<TextEntity> lapsText;
    std::shared_ptr<TextEntity> speedText;
    std::shared_ptr<TextEntity> placeText;
    std::shared_ptr<TextEntity> actualPlaceText;
    std::shared_ptr<TextEntity> countdownText;
    std::shared_ptr<ImageEntity> speedometer;
    std::shared_ptr<ImageEntity> dial;

    std::shared_ptr<ImageButton> exitButton;
    MainMenu &menu;

    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_HUD_H
