#ifndef CSE165PROJECT_COMMAND_H
#define CSE165PROJECT_COMMAND_H

#include "GLWidget.h"

QT_FORWARD_DECLARE_CLASS(GLWidget)

class Command {
public:
    virtual ~Command() = default;
    virtual void init(GLWidget& widget) = 0;
    virtual void draw(GLWidget& widget) = 0;
    virtual bool isFinished(GLWidget& widget) = 0;
    virtual float getPriority() {
        return 0;
    };
};

#endif //CSE165PROJECT_COMMAND_H
