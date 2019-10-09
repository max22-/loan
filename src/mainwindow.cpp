#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include "config.h"
#include "recordingsmodel.h"
#include <QSortFilterProxyModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->recordingSlider->setMaxTime(Config::getInstance().maxRecordingTimeS()*1000);
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
    connectButton(ui->playButtonPlayer, QList<const char*>({"stop", "play"}));
    connectButton(ui->stopButtonPlayer, "stop");

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
    connectState("SaveMessageState", &StateHandler::saveMessageState);
    connectState("SavedMessageState", &StateHandler::savedMessageState);
    connectState("ListeningMessageState2", &StateHandler::listeningMessageState2);

    stateMachine.start();

    connect(&audioRecorder, &AudioRecorder::lengthChanged, ui->recordingSlider, &TimeSlider::setMaxTime);
    connect(&audioRecorder, &AudioRecorder::positionChanged, ui->recordingSlider, &TimeSlider::setTime);

    connect(&audioRecorder, &AudioRecorder::stateChanged, [this](AudioRecorderState newState) {
        if(newState == AudioRecorderState::IDLE)
            stateMachine.submitEvent("stop");
    });

    recordingsModel = new RecordingsModel(nullptr);
    proxyModel.setSourceModel(recordingsModel);
    ui->tableView->setModel(&proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->show();

    proxyModel.setFilterKeyColumn(NICKNAME_COLUMN);
    connect(ui->nicknameFilterEdit, &QLineEdit::textChanged, &proxyModel, &QSortFilterProxyModel::setFilterFixedString);

    mediaPlayer.setNotifyInterval(10);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, ui->playerSlider, &TimeSlider::setMaxTime);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, ui->playerSlider, &TimeSlider::setTime);
    connect(&mediaPlayer, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State mpstate) {
        if(mpstate == QMediaPlayer::StoppedState)
            stateMachine.submitEvent("stop");
    });
}

MainWindow::~MainWindow()
{
    delete stateHandler;
    delete ui;
    delete recordingsModel;
}

void MainWindow::connectButton(const QPushButton *button, const char *eventName) {
    connect(button, &QPushButton::clicked, [this, button, eventName]() {
        qDebug() << ("Button \"" + button->objectName() + "\" clicked").toStdString().c_str();
        this->stateMachine.submitEvent(eventName);
    });
}

void MainWindow::connectButton(const QPushButton *button, const QList<const char *> eventNames) {
    foreach (const char* eventName, eventNames)
        connectButton(button, eventName);
}

void MainWindow::connectState(const char *stateName, void (StateHandler::*stateHandlerMember)(bool) ) {
    stateMachine.connectToState(stateName, stateHandler, stateHandlerMember);
}
