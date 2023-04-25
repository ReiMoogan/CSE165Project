#ifndef CSE165PROJECT_IMAGEBUTTON_H
#define CSE165PROJECT_IMAGEBUTTON_H

#include "ImageEntity.h"

class ImageButton : public ImageEntity {
public:
    explicit ImageButton(const QString &imagePath);
    ImageButton(const QString& imagePath, float x, float y, float z);
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
    std::function<void()> onClick;
private:
    bool mousePreviouslyPressed = false;
    bool isFinishedFlag = false;
    bool mouseOver(GLWidget &widget);
};


#endif //CSE165PROJECT_IMAGEBUTTON_H
