#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include "config.h"
#include "recordingsmodel.h"
#include <QSortFilterProxyModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->recordingSlider->setMinimum(0);
    ui->recordingSlider->setMaximum(Config::getInstance().maxRecordingTimeS()*1000);
    stateHandler = new StateHandler(ui, &stateMachine, this);

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
    connectButton(ui->validateButton2, "validate");

    connectState("HomeState", &StateHandler::homeState);
    connectState("TocState", &StateHandler::tocState);
    connectState("FormState", &StateHandler::formState);
    connectState("ListenHomeState", &StateHandler::listenHomeState);
    connectState("ValidateFormState", &StateHandler::validateFormState);
    connectState("RecordState", &StateHandler::recordState);
    connectState("RecordHomeState", &StateHandler::recordHomeState);
    connectState("ValidateCancelState", &StateHandler::validateCancelState);
    connectState("RecordingState", &StateHandler::recordingState);
    connectState("RecordedMessageState", &StateHandler::recordedMessageState);
    connectState("ListeningMessageState", &StateHandler::listeningMessageState);
    connectState("ReRecordState", &StateHandler::reRecordState);
    connectState("ValidateMessageState", &StateHandler::validateMessageState);
    connectState("MP3ConversionState", &StateHandler::MP3ConversionState);
    connectState("SaveMessageState", &StateHandler::saveMessageSate);
    connectState("SavedMessageState", &StateHandler::savedMessageSate);

    stateMachine.start();

    connect(&audioRecorder, &AudioRecorder::positionChanged, [this](int newPosition) {
        setRecordingSliderPosition(newPosition);
    });

    connect(&audioRecorder, &AudioRecorder::lengthChanged, [this](int newLength) {
        ui->recordingSlider->setMaximum(newLength);
    });

    connect(&audioRecorder, &AudioRecorder::stateChanged, [this](AudioRecorderState newState) {
        if(newState == AudioRecorderState::IDLE)
            stateMachine.submitEvent("stop");
    });

    recordingsModel = new RecordingsModel(nullptr);
    proxyModel.setSourceModel(recordingsModel);
    ui->tableView->setModel(&proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->show();

    proxyModel.setFilterKeyColumn(NICKNAME_COLUMN);
    connect(ui->nicknameFilterEdit, &QLineEdit::textChanged, &proxyModel, &QSortFilterProxyModel::setFilterFixedString);
}

MainWindow::~MainWindow()
{
    delete stateHandler;
    delete ui;
    delete recordingsModel;
}

void MainWindow::setRecordingSliderPosition(int ms) {  // parameter "ms" in milliseconds
    ui->recordingSlider->setValue(ms);
    QTime time = QTime(0, 0, 0, 0).addMSecs(ms);
    ui->recordingDurationLabel->setText(time.toString("mm:ss.zzz"));
}

void MainWindow::setPlayerSliderPosition(int ms) {  // parameter "ms" in milliseconds
    ui->playerSlider->setValue(ms);
    QTime time = QTime(0, 0, 0, 0).addMSecs(ms);
    ui->playerTimeLabel->setText(time.toString("mm:ss.zzz"));
}

void MainWindow::connectButton(const QPushButton *button, const char *eventName) {
    connect(button, &QPushButton::clicked, [this, eventName]() { this->stateMachine.submitEvent(eventName); });
}

void MainWindow::connectState(const char *stateName, void (StateHandler::*stateHandlerMember)(bool) ) {
    stateMachine.connectToState(stateName, stateHandler, stateHandlerMember);
}
