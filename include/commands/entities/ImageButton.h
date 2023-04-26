#ifndef CSE165PROJECT_IMAGEBUTTON_H
#define CSE165PROJECT_IMAGEBUTTON_H

#include <QSoundEffect>
#include "ImageEntity.h"

class ImageButton : public ImageEntity {
public:
    explicit ImageButton(const QString &imagePath);
    ImageButton(const QString& imagePath, float x, float y, float z);
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    void select();
    void deselect();
    std::function<void()> onClick;
private:
    bool lastSelectionState = false;
    bool mousePreviouslyPressed = false;
    bool mouseOver(GLWidget &widget);
    static bool soundInitialized;
    static QSoundEffect clickSound;
};


#endif //CSE165PROJECT_IMAGEBUTTON_H
