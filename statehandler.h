#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <QObject>
#include "ui_mainwindow.h"
#include "statechart.h"

class StateHandler : public QObject
{
    Q_OBJECT
public:
    explicit StateHandler(Ui::MainWindow *ui, Statechart *stateMachine, QObject *parent = nullptr);

signals:

public slots:
    void homeState(bool);
    void tocState(bool);
    void recordFormState(bool);
    void listenHomeState(bool);
    void validateFormState(bool);
    void dataEnteredState(bool);

private:
    Ui::MainWindow *ui;
    Statechart *stateMachine;
};

#endif // STATEHANDLER_H
