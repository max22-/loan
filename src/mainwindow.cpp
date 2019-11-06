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
    connectButton(ui->playButtonPlayer, "play");
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
    connectState("WaitForRecorderState", &StateHandler::waitForRecorderState);
    connectState("ListeningMessageState", &StateHandler::listeningMessageState);
    connectState("ReRecordState", &StateHandler::reRecordState);
    connectState("ValidateMessageState", &StateHandler::validateMessageState);
    connectState("MP3ConversionState", &StateHandler::MP3ConversionState);
    connectState("SaveMessageState", &StateHandler::saveMessageState);
    connectState("SavedMessageState", &StateHandler::savedMessageState);
    connectState("ListeningMessageState2", &StateHandler::listeningMessageState2);
    connectState("WaitForPlayerState", &StateHandler::waitForPlayerState);

    stateMachine.start();

    connect(&audioRecorder, &AudioRecorder::lengthChanged, ui->recordingSlider, &TimeSlider::setMaxTime);
    connect(&audioRecorder, &AudioRecorder::positionChanged, ui->recordingSlider, &TimeSlider::setTime);

    connect(&audioRecorder, &AudioRecorder::stateChanged, [this](AudioRecorderState newState) {
        if(newState == AudioRecorderState::IDLE)
            stateMachine.submitEvent("recorderStopped");
    });

    recordingsModel = new RecordingsModel(nullptr);
    proxyModel.setSourceModel(recordingsModel);
    ui->tableView->setModel(&proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(DATE_COLUMN, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(NICKNAME_COLUMN, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(AGE_COLUMN, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(CITY_COLUMN, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EVALUATION_COLUMN, QHeaderView::ResizeToContents);
    ui->tableView->show();

    proxyModel.setFilterKeyColumn(NICKNAME_COLUMN);
    connect(ui->nicknameFilterEdit, &QLineEdit::textChanged, &proxyModel, &QSortFilterProxyModel::setFilterFixedString);

    connect(&mediaPlayer, &MediaPlayerWrapper::durationChanged, ui->playerSlider, &TimeSlider::setMaxTime);
    connect(&mediaPlayer, &MediaPlayerWrapper::positionChanged, ui->playerSlider, &TimeSlider::setTime);
    connect(&mediaPlayer, &MediaPlayerWrapper::playerStopped, [this]() {
        stateMachine.submitEvent("playerStopped");
    });
    connect(&mediaPlayer, &MediaPlayerWrapper::playingNewMessage, [this](int messageNumber) {
        JsonFile jsonFile = mediaPlayer.getFiles().at(messageNumber);
        ui->currentPlayingMessageLabel->setText("Message de " + jsonFile.getNickname() + ", " + QString::number(jsonFile.getAge()) + " ans, de " + jsonFile.getCity());
    });

    // Media player volume control
    connect(ui->playerVolumeSlider, &QSlider::valueChanged, [this](int logarithmicVolume) {
        qreal linearVolume = QAudio::convertVolume(logarithmicVolume / qreal(100.0),
                                                   QAudio::LogarithmicVolumeScale,
                                                   QAudio::LinearVolumeScale);
        mediaPlayer.setVolume(qRound(linearVolume * 100));
        ui->playerVolumeLabel->setText(QString::number(logarithmicVolume) + "%");
    });

    connect(ui->togglePlaylistButton, &QPushButton::clicked, [this]() {
        if(ui->playlist->isVisible())
            ui->playlist->setVisible(false);
        else
            ui->playlist->setVisible(true);
    });

    // Playlist
    playlistModel = new PlaylistModel(mediaPlayer, nullptr);
    ui->playlistView->setModel(playlistModel);
}

MainWindow::~MainWindow()
{
    delete stateHandler;
    delete ui;
    delete recordingsModel;
}

void MainWindow::connectButton(const QPushButton *button, const char *eventName) {
    connect(button, &QPushButton::clicked, [this, button, eventName]() {
        qDebug() << ("Button \"" + button->objectName() + "\" clicked, sending event \"" + eventName + "\"").toStdString().c_str();
        this->stateMachine.submitEvent(eventName);
    });
}

void MainWindow::connectState(const char *stateName, void (StateHandler::*stateHandlerMember)(bool) ) {
    stateMachine.connectToState(stateName, stateHandler, stateHandlerMember);
}
