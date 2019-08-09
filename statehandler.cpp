#include <QMessageBox>

#include <QFile>

#include <QTimer>
#include "statehandler.h"
#include "mainwindow.h"
#include "config.h"
#include <QProcess>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

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
        auto *nicknameValidator = new QRegExpValidator(QRegExp("^(?!\s*$).+"), nullptr);  // regex found on stackoverflow https://stackoverflow.com/questions/3085539/regular-expression-for-anything-but-an-empty-string
        auto *ageValidator = new QIntValidator(0, 200);
        auto *cityValidator = new QRegExpValidator(QRegExp("^(?!\s*$).+$"), nullptr);
        QMessageBox msgBox;
        bool errorFlag = false;
        msgBox.setWindowTitle("Erreur de saisie");
        std::string errorMessage;
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
        if(errorFlag) {
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
    else {
        qDebug() << "*** Quitting validateFormState ***";
        qDebug() << "nickname = " << ui->nicknameLineEdit->text();
        qDebug() << "age = " << ui->ageLineEdit->text();
        qDebug() << "city = " << ui->cityLineEdit->text();
        qDebug() << "evaluation = " << ui->evaluationSlider->value();
        qDebug() << "**********************************";
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
        qDebug() << "validateCancel1State";
        confirmationMessageBox("Vous avez enregistré des informations.", "Voulez-vous vraiment tout supprimer et revenir à l'écran d'accueil ?");
    }
}

void StateHandler::recordingState(bool active) {
    if (active) {
        qDebug() << "entering recordingState";
        mainWindow->audioRecorder.clear();
        mainWindow->audioRecorder.startRecording();
    }
    else {
        mainWindow->audioRecorder.stop();
        qDebug() << "quitting recordingState";
    }
}

void StateHandler::recordedMessageState(bool active) {
    if (active) {
        qDebug() << "recordedMessageState";
    }
}

void StateHandler::listeningMessageState(bool active) {
    if (active) {
        qDebug() << "listeningMessageState";
        mainWindow->audioRecorder.startPlaying();
    }
    else {
        mainWindow->audioRecorder.stop();
        qDebug() << "quitting listeningMessageState";
    }
}

void StateHandler::reRecordState(bool active) {
    if(active) {
        qDebug() <<"reRecordState";
        confirmationMessageBox("Vous avez enregistré un message.", "Voulez-vous vraiment l'effacer pour le réenregistrer à nouveau ?");
    }
}

void StateHandler::validateMessageState(bool active) {
    if(active) {
        qDebug() << "validateMessageState";
        confirmationMessageBox("Vous avez enregistré un message ainsi que des informations.", "Êtes-vous sûr de vouloir les valider ? (ceci est définitif)");
    }
}

void StateHandler::MP3ConversionState(bool active) {
    QMessageBox msgBox;
    if(active) {
        qDebug() << "MP3ConversionState";
        ui->stackedWidget->setCurrentIndex(4);
        int retCode = QProcess::execute(Config::getInstance().MP3ConversionCommand(mainWindow->audioRecorder.getFormat()));
        if( retCode != 0) {
            qCritical() << "MP3 conversion failed. retCode = " << retCode;
            msgBox.setText("La conversion au format MP3 a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
            return;
        }
        if(!QFile(Config::getInstance().tempMP3FileName()).exists()) {
            qCritical() << "No MP3 file has been produced.";
            msgBox.setText("La conversion au format MP3 a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
            return;
        }
        msgBox.setText("Ca marche !");
        msgBox.exec();
        stateMachine->submitEvent("save");
    }
}

void StateHandler::saveMessageSate(bool active) {
    if(active) {
        qDebug() << "entering saveMessageState";
        QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QJsonObject jsonObject;
        jsonObject.insert("timeStamp", timeStamp);
        jsonObject.insert("nickname", ui->nicknameLineEdit->text());
        jsonObject.insert("filename", timeStamp + ".mp3");
        jsonObject.insert("age", ui->ageLineEdit->text().toInt());
        jsonObject.insert("city", ui->cityLineEdit->text());
        jsonObject.insert("evaluation", ui->evaluationSlider->value());
        QJsonDocument jsonDocument(jsonObject);
        qDebug() << jsonDocument.toJson();

        Config& config = Config::getInstance();
        bool moved = QFile(Config::getInstance().tempMP3FileName()).rename(config.outboxDirectory().absoluteFilePath(timeStamp + ".mp3"));
        if(!moved) {
            qCritical() << "Couldn't move MP3 file to outbox directory : " + config.outboxDirectory().absolutePath();
            QMessageBox msgBox;
            msgBox.setText("L'enregistrement du message dans la base de données a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
            return;
        }

        QFile jsonFile(config.outboxDirectory().absoluteFilePath(timeStamp + ".json"));
        if(jsonFile.open(QFile::WriteOnly)) {
            jsonFile.write(jsonDocument.toJson());
            jsonFile.close();
        }
        else {
            qCritical() << "Could'nt create json file in outbox directory.";
            QMessageBox msgBox;
            msgBox.setText("L'enregistrement du message dans la base de données a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
        }

    }
}



void StateHandler::confirmationMessageBox(const QString& text, const QString& informativeText) {
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
        qDebug() << "msgBox.exec() returned something other than yes or no.";
        break;
    }
}
