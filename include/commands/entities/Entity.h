#ifndef CSE165PROJECT_ENTITY_H
#define CSE165PROJECT_ENTITY_H

#include "commands/Command.h"

enum DrawMode {
    CORNER,
    CENTER
};

class Entity : public Command {
protected:
    float xRot = 0;
    float yRot = 0;
    float zRot = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    float xScale = 1;
    float yScale = 1;
    DrawMode mode = CENTER;
    // no z scale
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
    virtual void setTranslation(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    virtual void setRotation(float xRot, float yRot, float zRot) {
        this->xRot = xRot;
        this->yRot = yRot;
        this->zRot = zRot;
    }

    virtual void setScale(float xScale, float yScale) {
        this->xScale = xScale;
        this->yScale = yScale;
    }

    virtual void setDrawMode(DrawMode mode) {
        // lol basically a setter method
        this->mode = mode;
    }

public:
    [[nodiscard]] float getX() const {
        return x;
    }

    [[nodiscard]] float getY() const {
        return y;
    }

    [[nodiscard]] float getZ() const {
        return z;
    }

    [[nodiscard]] float getPriority() override {
        return z;
    }
#pragma clang diagnostic pop

};

#endif //CSE165PROJECT_ENTITY_H
