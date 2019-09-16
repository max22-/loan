#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statechart.h"
#include "statehandler.h"
#include "audiorecorder.h"
#include "recordingsmodel.h"
#include <QSortFilterProxyModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

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

private:
    Ui::MainWindow *ui;
    Statechart stateMachine;
    StateHandler *stateHandler;
    AudioRecorder audioRecorder;
    RecordingsModel *recordingsModel;
    QSortFilterProxyModel proxyModel;
    QMediaPlayer mediaPlayer;
    QMediaPlaylist mediaPlayList;

    void connectButton(const QPushButton *, const char *);
    void connectState(const char *, void (StateHandler::*)(bool));

friend class StateHandler;
};

#endif // MAINWINDOW_H
