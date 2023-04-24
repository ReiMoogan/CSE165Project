#ifndef CSE165PROJECT_IMAGEBUTTON_H
#define CSE165PROJECT_IMAGEBUTTON_H

#include "ImageEntity.h"

class ImageButton : public ImageEntity {
public:
    void init(GLWidget &widget) override;
    void draw(GLWidget &widget) override;
    bool isFinished(GLWidget &widget) override;
private:
    bool isFinishedFlag = false;
};


#endif //CSE165PROJECT_IMAGEBUTTON_H
