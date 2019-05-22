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
    void tocState(bool);

private:
    Ui::MainWindow *ui;
};

#endif // STATEHANDLER_H
