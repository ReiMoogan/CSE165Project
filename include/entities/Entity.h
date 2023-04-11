#ifndef CSE165PROJECT_ENTITY_H
#define CSE165PROJECT_ENTITY_H

#include <QString>
#include "GLWidget.h"

QT_FORWARD_DECLARE_CLASS(GLWidget)

class Entity {
public:
    virtual ~Entity() = default;
    virtual void init(GLWidget& widget) = 0;
    virtual void draw(GLWidget& widget) = 0;
    virtual bool isFinished(GLWidget& widget) = 0;
    virtual float getZ() {
        return 0;
    };
};

#endif //CSE165PROJECT_ENTITY_H
