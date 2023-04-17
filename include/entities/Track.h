#ifndef CSE165PROJECT_TRACK_H
#define CSE165PROJECT_TRACK_H

#include "ImageEntity.h"
#include "Derek.h"

class Track : public ImageEntity {
private:
    Derek *car;
public:
    Track(Derek* _car);
    void init(GLWidget& widget) override;
    void draw(GLWidget& widget) override;
    bool isFinished(GLWidget& widget) override;
};


#endif //CSE165PROJECT_TRACK_H
