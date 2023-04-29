#ifndef CSE165PROJECT_CHECKPOINT_H
#define CSE165PROJECT_CHECKPOINT_H

#include "ImageEntity.h"

class Checkpoint : public ImageEntity {
public:
    int index;
    static const float checkpointCoords[][2];
    static const int totalCheckpoints;
    explicit Checkpoint(int);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
};

#endif //CSE165PROJECT_CHECKPOINT_H
