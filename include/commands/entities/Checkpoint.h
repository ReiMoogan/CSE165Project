#ifndef CSE165PROJECT_CHECKPOINT_H
#define CSE165PROJECT_CHECKPOINT_H

#include "ImageEntity.h"

class Checkpoint : public ImageEntity {
public:
    int index;
    Checkpoint(int);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
};


#endif //CSE165PROJECT_CHECKPOINT_H
