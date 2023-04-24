#include "commands/MainMenu.h"

void MainMenu::init(GLWidget &widget) {

}

void MainMenu::draw(GLWidget &widget) {

}

bool MainMenu::isFinished(GLWidget &widget) {
    return false;
}

float MainMenu::getPriority() {
    // we want this to be the first thing called (check isFinished())
    return 694201337.0f; // nice
}
