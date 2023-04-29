#ifndef CSE165PROJECT_MAINMENU_H
#define CSE165PROJECT_MAINMENU_H

#include "GLWidget.h"
#include "commands/entities/ImageEntity.h"
#include "commands/entities/ImageButton.h"
#include "commands/entities/TextEntity.h"
#include "commands/entities/HUD.h"
#include "commands/entities/Map.h"

#include <QMediaPlayer>
#include <QAudioOutput>

class HUD;
class MainMneu;
class Map;

class MainMenu : public Command {
public:
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
    float getPriority() override;
    void showMenuScreen();

    friend class HUD;
private:
    QMediaPlayer music;
    QAudioOutput output;
    bool startGame = false;
    bool realizedStartGame = false;
    bool endGame = true;
    bool realizedEndGame = false;
    std::shared_ptr<Map> map;
    std::shared_ptr<HUD> hud;
    std::shared_ptr<TextEntity> fps;
    std::shared_ptr<ImageEntity> background;
    std::shared_ptr<ImageEntity> title;
    std::shared_ptr<ImageButton> startButton;
    std::shared_ptr<ImageButton> exitButton;
};


#endif //CSE165PROJECT_MAINMENU_H
