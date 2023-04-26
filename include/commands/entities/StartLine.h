#ifndef CSE165PROJECT_STARTLINE_H
#define CSE165PROJECT_STARTLINE_H

#include "ImageEntity.h"

class StartLine : public ImageEntity {
public:
    StartLine();
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    friend class Map;
};


#endif //CSE165PROJECT_STARTLINE_H
