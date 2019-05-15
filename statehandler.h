#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <QObject>

class StateHandler : public QObject
{
    Q_OBJECT
public:
    explicit StateHandler(QObject *parent = nullptr);

signals:

public slots:
    void tocState(bool);
};

#endif // STATEHANDLER_H
