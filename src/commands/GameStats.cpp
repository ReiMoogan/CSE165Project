#include "commands/GameStats.h"

#include <QMessageBox>

/*
Example usage:

auto testStatsData = StatData {
    {QTime(0, 2, 14), QTime(0, 1, 13), QTime(0, 4, 20) },
    40,
    20
};
addCommand(std::make_shared<GameStats>(testStatsData));
 */

GameStats::GameStats(const StatData& data) {
    message = "You've finished in " + data.place + " place!\n\n";
    message += "Lap Times:\n";
    for (int i = 0; i < data.lapTimes.size(); ++i) {
        message += "- Lap " + std::to_string(i + 1) + ": " + data.lapTimes[i].toString("mm:ss.zzz").toStdString() + "\n";
    }
    message += "\nMax Speed: " + std::to_string(data.maxSpeed) + " m/s\n";
    message += "Average Speed: " + std::to_string(data.avgSpeed) + " m/s\n";
}

void GameStats::init(GLWidget &widget) {
    auto* msgBox = new QMessageBox(QMessageBox::Information,
                       "Game Statistics",
                       "You've finished a race!",
                       QMessageBox::Ok | QMessageBox::Escape);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setInformativeText(QString::fromStdString(message));
    msgBox->setModal(false);
    msgBox->open();
}

void GameStats::draw(GLWidget &widget) {

}

bool GameStats::isFinished(GLWidget &widget) {
    return true;
}
