#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    stateHandler = new StateHandler(ui, &stateMachine);

    connect(ui->evaluationSlider, &QSlider::valueChanged, [this](int newValue) {ui->evaluationLabel->setText(QString::number(newValue)); } );

    connectButton(ui->tocButton, "toc");
    connectButton(ui->recordHomeButton, "record");
    connectButton(ui->validateButton, "validate");
    connectButton(ui->listenHomeButton, "listen");
    connectButton(ui->backButton1, "back");
    connectButton(ui->backButton2, "back");
    connectButton(ui->backButton3, "back");
    connectButton(ui->recordButton, "record");
    connectButton(ui->stopButton, "stop");
    connectButton(ui->playButton, "play");
    connectButton(ui->cancelButton, "cancel");

    connectState("HomeState", &StateHandler::homeState);
    connectState("TocState", &StateHandler::tocState);
    connectState("FormState", &StateHandler::formState);
    connectState("ListenHomeState", &StateHandler::listenHomeState);
    connectState("ValidateFormState", &StateHandler::validateFormState);
    connectState("RecordState", &StateHandler::recordState);
    connectState("RecordHomeState", &StateHandler::recordHomeState);
    connectState("ValidateCancel1State", &StateHandler::validateCancel1State);
    connectState("RecordingState", &StateHandler::recordingState);
    connectState("RecordedMessageState", &StateHandler::recordedMessageState);
    connectState("ListeningMessageState", &StateHandler::listeningMessageState);
    connectState("ValidateCancel2State", &StateHandler::validateCancel2State);

    stateMachine.start();

}

MainWindow::~MainWindow()
{
    delete stateHandler;
    delete ui;
}

void MainWindow::connectButton(const QPushButton *button, const char *eventName) {
    connect(button, &QPushButton::clicked, [this, eventName]() { this->stateMachine.submitEvent(eventName); });
}

void MainWindow::connectState(const char *stateName, void (StateHandler::*stateHandlerMember)(bool) ) {
    stateMachine.connectToState(stateName, stateHandler, stateHandlerMember);
}
