#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statechart.h"
#include "statehandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRecordingSliderPosition(int);   // parameter "ms" in milliseconds

private:
    Ui::MainWindow *ui;
    Statechart stateMachine;
    StateHandler *stateHandler;

    void connectButton(const QPushButton *, const char *);
    void connectState(const char *, void (StateHandler::*)(bool));
};

#endif // MAINWINDOW_H
