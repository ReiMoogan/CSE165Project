#ifndef CSE165PROJECT_COMMAND_H
#define CSE165PROJECT_COMMAND_H

#include "GLWidget.h"

QT_FORWARD_DECLARE_CLASS(GLWidget)

class Command : public QObject {
public:
    virtual void init(GLWidget& widget) = 0;
    virtual void draw(GLWidget& widget) = 0;
    virtual bool isFinished(GLWidget& widget) = 0;
    virtual float getPriority() {
        return 0;
    };
    bool forceFinish = false;
};

#endif //CSE165PROJECT_COMMAND_H
