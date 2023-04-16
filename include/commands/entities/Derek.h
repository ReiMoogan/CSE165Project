#ifndef CSE165PROJECT_DEREK_H
#define CSE165PROJECT_DEREK_H

#include "ImageEntity.h"

class Derek : public ImageEntity {
public:
    Derek();
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_DEREK_H
