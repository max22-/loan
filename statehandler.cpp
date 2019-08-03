#include <QMessageBox>

#include <QFile>

#include <QTimer>
#include "statehandler.h"
#include "mainwindow.h"
#include "config.h"
#include <QProcess>
#include <QDebug>

StateHandler::StateHandler(Ui::MainWindow *ui, Statechart *stateMachine, MainWindow *mainWindow, QObject *parent) : QObject(parent)
{
    this->ui = ui;
    this->stateMachine = stateMachine;
    this->mainWindow = mainWindow;
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
        mainWindow->audioRecorder.clear();
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
        mainWindow->audioRecorder.clear();
        mainWindow->audioRecorder.startRecording();
    }
    else {
        mainWindow->audioRecorder.stop();
        qDebug("quitting recordingState");
    }
}

void StateHandler::recordedMessageState(bool active) {
    if (active) {
        qDebug("recordedMessageState");
    }
}

void StateHandler::listeningMessageState(bool active) {
    if (active) {
        qDebug("listeningMessageState");
        mainWindow->audioRecorder.startPlaying();
    }
    else {
        mainWindow->audioRecorder.stop();
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
    QMessageBox msgBox;
    if(active) {
        qDebug("MP3ConversionState");
        ui->stackedWidget->setCurrentIndex(4);
        int retCode = QProcess::execute(Config::getInstance().MP3ConversionCommand());
        if( retCode != 0) {
            qCritical() << "MP3 conversion failed. retCode = " << retCode;
            msgBox.setText("La conversion au format MP3 a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
            return;
        }
        if(QFile(Config::getInstance().tempMP3AudioFileName()).exists() == false) {
            qCritical() << "No MP3 file has been produced.";
            msgBox.setText("La conversion au format MP3 a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
            return;
        }
        msgBox.setText("Ca marche !");
        msgBox.exec();

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
