#ifndef CSE165PROJECT_MAINMENU_H
#define CSE165PROJECT_MAINMENU_H

#include "GLWidget.h"

class MainMenu : public Command {
public:
    void init(GLWidget &widget) override;

    void draw(GLWidget &widget) override;

    bool isFinished(GLWidget &widget) override;

    float getPriority() override;
};


#endif //CSE165PROJECT_MAINMENU_H
