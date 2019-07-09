#include <QMessageBox>
#include <QAudioDeviceInfo>
#include <QFile>
#include <QAudioOutput>
#include <QTimer>
#include "statehandler.h"
#include "mainwindow.h"
#include "config.h"

StateHandler::StateHandler(Ui::MainWindow *ui, Statechart *stateMachine, MainWindow *mainWindow, QObject *parent) : QObject(parent)
{
    this->ui = ui;
    this->stateMachine = stateMachine;
    this->mainWindow = mainWindow;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, [stateMachine]() {
        stateMachine->submitEvent("stop");
    });
    Config& config = Config::getInstance();

    // Audio ******************************
    // Audio Initialization *********************
    tempAudioFile.setFileName(config.tempAudioFileName());
    QAudioFormat format;
    format.setSampleRate(config.sampleRate());
    format.setChannelCount(config.channelCount());
    format.setSampleSize(config.sampleSize());
    format.setCodec(config.audioCodec());
    format.setByteOrder(config.byteOrder());
    format.setSampleType(config.sampleType());
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qDebug("Default format not supported, trying to use the nearest.");
        format = info.nearestFormat(format);
    }
    ui->recordingSlider->setMinimum(0);
    ui->recordingSlider->setMaximum(config.maxRecordingTimeS()*1000);

    // Audio Input *************************
    audioInput = new QAudioInput(format, this);
    audioInput->setNotifyInterval(10);
    connect(audioInput, &QAudioInput::notify, [mainWindow, this]() {
        int ms = static_cast<int>(audioInput->processedUSecs()/1000);
        mainWindow->setRecordingSliderPosition(ms);
    });
    // *************************************
    // Audio Output ************************
    audioOutput = new QAudioOutput(format, this);
    audioOutput->setNotifyInterval(10);
    connect(audioOutput, &QAudioOutput::notify, [mainWindow, this]() {
        int ms = static_cast<int>(audioOutput->processedUSecs()/1000);
        mainWindow->setRecordingSliderPosition(ms);
    });
    connect(audioOutput, &QAudioOutput::stateChanged, [stateMachine, this](QAudio::State newState) {
        switch(newState) {
            case QAudio::IdleState:
                stateMachine->submitEvent("stop");
                break;
            case QAudio::StoppedState:
                stateMachine->submitEvent("stop");
                if(audioOutput->error() != QAudio::NoError)
                    qDebug("Error during file paying.");
                break;
            default:
                break;
        }
    });
    // *************************************

}

StateHandler::~StateHandler(void) {
    delete audioInput;
}



void StateHandler::homeState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->nicknameLineEdit->setText("");
        ui->ageLineEdit->setText("");
        ui->cityLineEdit->setText("");
        ui->evaluationSlider->setValue(0);
    }
}


void StateHandler::tocState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void StateHandler::formState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void StateHandler::listenHomeState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(6);
    }
}

void StateHandler::validateFormState(bool active) {
    if(active) {
        QString nickname = ui->nicknameLineEdit->text();
        QString age = ui->ageLineEdit->text();
        QString city = ui->cityLineEdit->text();
        int pos = 0;
        QRegExpValidator *nicknameValidator = new QRegExpValidator(QRegExp("^(?!\s*$).+"), nullptr);  // regex found on stackoverflow https://stackoverflow.com/questions/3085539/regular-expression-for-anything-but-an-empty-string
        QIntValidator *ageValidator = new QIntValidator(0, 200);
        QRegExpValidator *cityValidator = new QRegExpValidator(QRegExp("^(?!\s*$).+$"), nullptr);
        QMessageBox msgBox;
        bool errorFlag = false;
        msgBox.setWindowTitle("Erreur de saisie");
        std::string errorMessage = "";
        if(nicknameValidator->validate(nickname, pos) != QValidator::Acceptable) {
            errorFlag = true;
            errorMessage += "Veuillez entrer un pseudonyme.\n";
        }
        if(ageValidator->validate(age, pos) != QValidator::Acceptable) {
            errorFlag = true;
            errorMessage += "L'âge doit être compris entre 0 et 200.\n";
        }
        if(cityValidator->validate(city, pos) != QValidator::Acceptable) {
            errorFlag = true;
            errorMessage += "Veuillez entrer votre ville.\n";
        }
        if(ui->evaluationSlider->value() < 1 || ui ->evaluationSlider->value() > 5) {
            errorFlag = true;
            errorMessage += "Veuillez entrer une note entre 1 et 5 inclus.";
        }
        if(errorFlag == true) {
            msgBox.setText(errorMessage.c_str());
            msgBox.exec();
            stateMachine->submitEvent("notOk");
        }
        else
            stateMachine->submitEvent("ok");
        delete nicknameValidator;
        delete ageValidator;
        delete cityValidator;
    }
}

void StateHandler::recordState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void StateHandler::recordHomeState(bool active) {
    if (active) {
    }
}

void StateHandler::validateCancelState(bool active) {
    if(active) {
        qDebug("validateCancel1State");
        confirmationMessageBox("Vous avez enregistré des informations.", "Voulez-vous vraiment tout supprimer et revenir à l'écran d'accueil ?");
    }
}

void StateHandler::recordingState(bool active) {
    if (active) {
        qDebug("entering recordingState");
        ui->recordingSlider->setMaximum(Config::getInstance().maxRecordingTimeS()*1000);
        tempAudioFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        audioInput->start(&tempAudioFile);
        timer.start(Config::getInstance().maxRecordingTimeS()*1000);
    }
    else {
        timer.stop();
        audioInput->stop();
        tempAudioFile.close();
        qDebug("quitting recordingState");
    }
}

void StateHandler::recordedMessageState(bool active) {
    if (active) {
        qDebug("recordedMessageState");
        int ms = static_cast<int>(audioInput->processedUSecs()/1000);
        ui->recordingSlider->setMaximum(ms);
    }
}

void StateHandler::listeningMessageState(bool active) {
    if (active) {
        qDebug("listeningMessageState");
        tempAudioFile.open(QIODevice::ReadOnly);
        audioOutput->start(&tempAudioFile);
    }
    else {
        audioOutput->stop();
        tempAudioFile.close();
        qDebug("quitting listeningMessageState");
    }
}

void StateHandler::reRecordState(bool active) {
    if(active) {
        qDebug("reRecordState");
        confirmationMessageBox("Vous avez enregistré un message.", "Voulez-vous vraiment l'effacer pour le réenregistrer à nouveau ?");
    }
}

void StateHandler::MP3ConversionState(bool active) {
    if(active) {
        qDebug("MP3ConversionState");
        ui->stackedWidget->setCurrentIndex(4);
    }
}

void StateHandler::validateMessageState(bool active) {
    if(active) {
        qDebug("validateMessageState");
        confirmationMessageBox("Vous avez enregistré un message ainsi que des informations.", "Êtes-vous sûr de vouloir les valider ? (ceci est définitif)");
    }
}

void StateHandler::confirmationMessageBox(QString text, QString informativeText) {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Oui");
    msgBox.setButtonText(QMessageBox::No, "Non");
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch(ret) {
    case QMessageBox::Yes:
        stateMachine->submitEvent("yes");
        break;
    case QMessageBox::No:
        stateMachine->submitEvent("no");
        break;
    default:
        qDebug("msgBox.exec() returned something other than yes or no.");
        break;
    }
}
