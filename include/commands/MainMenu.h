#ifndef CSE165PROJECT_MAINMENU_H
#define CSE165PROJECT_MAINMENU_H

#include "GLWidget.h"
#include "commands/entities/ImageEntity.h"
#include "commands/entities/ImageButton.h"
#include "commands/entities/TextEntity.h"

class MainMenu : public Command {
public:
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
    float getPriority() override;
    void showMenuScreen();
private:
    bool startGame = false;
    bool realizedStartGame = false;
    bool endGame = true;
    bool realizedEndGame = false;
    std::shared_ptr<ImageEntity> title;
    std::shared_ptr<ImageButton> startButton;
    std::shared_ptr<ImageButton> exitButton;
    std::shared_ptr<TextEntity> startText;
    std::shared_ptr<TextEntity> exitText;
};


#endif //CSE165PROJECT_MAINMENU_H
