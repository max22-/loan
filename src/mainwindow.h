#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statechart.h"
#include "statehandler.h"
#include "audiorecorder.h"
#include "recordingsmodel.h"
#include <QSortFilterProxyModel>

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
    void setPlayerSliderPosition(int);   // parameter "ms" in milliseconds
    AudioRecorder audioRecorder;

private:
    Ui::MainWindow *ui;
    Statechart stateMachine;
    StateHandler *stateHandler;
    RecordingsModel *recordingsModel;
    QSortFilterProxyModel proxyModel;

    void connectButton(const QPushButton *, const char *);
    void connectState(const char *, void (StateHandler::*)(bool));
};

#endif // MAINWINDOW_H
