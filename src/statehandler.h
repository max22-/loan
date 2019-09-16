#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <QObject>
#include "ui_mainwindow.h"
#include "statechart.h"

class MainWindow;

class StateHandler : public QObject
{
    Q_OBJECT
public:
    explicit StateHandler(Ui::MainWindow *ui, Statechart *stateMachine, MainWindow *mainWindow, QObject *parent = nullptr);

signals:

public slots:
    void homeState(bool);
    void tocState(bool);
    void formState(bool);
    void listenHomeState(bool);
    void validateFormState(bool);
    void recordState(bool);
    void recordHomeState(bool);
    void validateCancelState(bool);
    void recordingState(bool);
    void recordedMessageState(bool);
    void listeningMessageState(bool);
    void reRecordState(bool);
    void validateMessageState(bool);
    void MP3ConversionState(bool);
    void saveMessageSate(bool);
    void savedMessageSate(bool);
    void listeningMessageState2(bool);

private:
    Ui::MainWindow *ui;
    Statechart *stateMachine;
    MainWindow *mainWindow;
    void confirmationMessageBox(const QString&, const QString&);
};

#endif // STATEHANDLER_H
