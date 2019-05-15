#include "statehandler.h"

StateHandler::StateHandler(QObject *parent) : QObject(parent)
{

}

void StateHandler::tocState(bool active) {
    if(active)
        qDebug("TOC!");
}
