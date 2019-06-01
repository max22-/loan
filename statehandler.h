#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <QObject>
#include "ui_mainwindow.h"

class StateHandler : public QObject
{
    Q_OBJECT
public:
    explicit StateHandler(Ui::MainWindow *ui, QObject *parent = nullptr);

signals:

public slots:
    void homeState(bool);
    void tocState(bool);
    void recordFormState(bool);
    void listenHomeState(bool);

private:
    Ui::MainWindow *ui;
};

#endif // STATEHANDLER_H
