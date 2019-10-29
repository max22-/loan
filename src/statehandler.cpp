#include <QMessageBox>

#include <QFile>

#include <QTimer>
#include "statehandler.h"
#include "mainwindow.h"
#include "config.h"
#include <QDebug>
#include <QDateTime>
#include "jsonfile.h"
#include <QFileInfo>
#include <QMediaPlayer>

StateHandler::StateHandler(Ui::MainWindow *ui, Statechart *stateMachine, MainWindow *mainWindow, QObject *parent) : QObject(parent)
{
    this->ui = ui;
    this->stateMachine = stateMachine;
    this->mainWindow = mainWindow;
}

void StateHandler::homeState(bool active) {
    if(active) {
        qDebug() << "Entering homeState";
        ui->stackedWidget->setCurrentIndex(0);
        ui->nicknameLineEdit->setText("");
        ui->ageLineEdit->setText("");
        ui->cityLineEdit->setText("");
        ui->evaluationSlider->setValue(0);
    }
    else {
        qDebug() << "Exiting homeState";
    }
}


void StateHandler::tocState(bool active) {
    if(active) {
        qDebug() << "Entering tocState";
        ui->stackedWidget->setCurrentIndex(1);
    }
    else {
        qDebug() << "Exiting tocState";
    }
}

void StateHandler::formState(bool active) {
    if(active) {
        qDebug() << "Entering formState";
        ui->stackedWidget->setCurrentIndex(2);
    }
    else {
        qDebug() << "Exiting formState";
    }
}

void StateHandler::listenHomeState(bool active) {
    if(active) {
        qDebug() << "Entering listenHomeState";
        ui->stackedWidget->setCurrentIndex(6);
        ui->playerVolumeSlider->setValue(Config::getInstance().initialPlayerVolume());  // Reset volume to its initial value
    }
    else {
        qDebug() << "Exiting listenHomeState";
    }
}

void StateHandler::validateFormState(bool active) {
    if(active) {
        qDebug() << "Entering validateFormState";
        QString nickname = ui->nicknameLineEdit->text();
        QString age = ui->ageLineEdit->text();
        QString city = ui->cityLineEdit->text();
        int pos = 0;
        auto *nicknameValidator = new QRegExpValidator(QRegExp("^(?!\s*$).+"), nullptr);  // regex found on stackoverflow https://stackoverflow.com/questions/3085539/regular-expression-for-anything-but-an-empty-string
        auto *ageValidator = new QIntValidator(0, 200);
        auto *cityValidator = new QRegExpValidator(QRegExp("^(?!\s*$).+$"), nullptr);
        bool errorFlag = false;
        QString errorMessage;
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
            qDebug() << "Invalid data in the form.";
            QMessageBox::warning(mainWindow, "Erreur de saisie", errorMessage);
            stateMachine->submitEvent("notOk");
        }
        else
            stateMachine->submitEvent("ok");
        delete nicknameValidator;
        delete ageValidator;
        delete cityValidator;
    }
    else {
        qDebug() << "Exiting validateFormState";
        qDebug() << "**********************************";
        qDebug() << "nickname = " << ui->nicknameLineEdit->text();
        qDebug() << "age = " << ui->ageLineEdit->text();
        qDebug() << "city = " << ui->cityLineEdit->text();
        qDebug() << "evaluation = " << ui->evaluationSlider->value();
        qDebug() << "**********************************";
    }
}

void StateHandler::recordState(bool active) {
    if(active) {
        qDebug() << "Entering recordState";
        ui->stackedWidget->setCurrentIndex(3);
    }
    else {
        qDebug() << "Exiting recordState";
    }
}

void StateHandler::recordHomeState(bool active) {
    if (active) {
        qDebug() << "Entering recordHomeState";
        mainWindow->audioRecorder.clear();
        ui->recorderStateLabel->setText("En attente...");
    }
    else {
        qDebug() << "Exiting recordHomeState";
    }
}

void StateHandler::validateCancelState(bool active) {
    if(active) {
        qDebug() << "Entering validateCancel1State";
        confirmationMessageBox("Vous avez enregistré des informations.", "Voulez-vous vraiment tout supprimer et revenir à l'écran d'accueil ?");
    }
    else {
        qDebug() << "Exiting validateCancelState";
    }
}

void StateHandler::recordingState(bool active) {
    if (active) {
        qDebug() << "entering recordingState";
        mainWindow->audioRecorder.clear();
        mainWindow->audioRecorder.startRecording();
        ui->recorderStateLabel->setText("Enregistrement en cours...");
    }
    else {
        mainWindow->audioRecorder.stop();
        qDebug() << "Exiting recordingState";
    }
}

void StateHandler::recordedMessageState(bool active) {
    if (active) {
        qDebug() << "Entering recordedMessageState";
        ui->recorderStateLabel->setText("Message enregistré.\nEn attente de validation...");
    }
    else {
        qDebug() << "Exiting recordedMessageState";
    }
}

void StateHandler::waitForRecorderState(bool active) {
    if (active) {
        qDebug() << "Entering waitForRecorderState";
        mainWindow->audioRecorder.stop();
    }
    else {
        qDebug() << "Exiting waitForRecorderState";
    }
}

void StateHandler::listeningMessageState(bool active) {
    if (active) {
        qDebug() << "Entering listeningMessageState";
        mainWindow->audioRecorder.startPlaying();
        ui->recorderStateLabel->setText("Ecoute du message");
    }
    else {
        mainWindow->audioRecorder.stop();
        qDebug() << "Exiting listeningMessageState";
    }
}

void StateHandler::reRecordState(bool active) {
    if(active) {
        qDebug() << "Entering reRecordState";
        confirmationMessageBox("Vous avez enregistré un message.", "Voulez-vous vraiment l'effacer pour le réenregistrer à nouveau ?");
    }
    else {
        qDebug() << "Exiting reRecordState";
    }
}

void StateHandler::validateMessageState(bool active) {
    if(active) {
        qDebug() << "Entering validateMessageState";
        confirmationMessageBox("Vous avez enregistré un message ainsi que des informations.", "Êtes-vous sûr de vouloir les valider ? (ceci est définitif)");
    }
    else {
        qDebug() << "Exiting validateMessageState";
    }
}

void StateHandler::MP3ConversionState(bool active) {
    if(active) {
        qDebug() << "Entering MP3ConversionState";
        ui->stackedWidget->setCurrentIndex(4);
        try {
            mainWindow->audioRecorder.convertToMP3();
            QMessageBox::information(mainWindow, "Information", "Ca marche !");
            stateMachine->submitEvent("save");
        } catch(QString msg) {
            qCritical() << msg;
            QMessageBox::critical(mainWindow, "Erreur", "La conversion au format MP3 a échoué, nous en sommes désolés.");
            stateMachine->submitEvent("error");
        }
    }
    else {
        qDebug() << "Exiting MP3ConversionState";
    }
}

void StateHandler::saveMessageState(bool active) {
    if(active) {
        qDebug() << "Entering saveMessageState";
        QDateTime timeStamp = QDateTime::currentDateTime();
        QString fileNamePrefix = timeStamp.toString(Config::getInstance().fileNameFormat());

        Config& config = Config::getInstance();
        JsonFile jsonFile(config.outboxDirectory().absoluteFilePath(fileNamePrefix + ".json"));
        jsonFile.setNickName(ui->nicknameLineEdit->text())
            .setAge(ui->ageLineEdit->text().toInt())
            .setCity(ui->cityLineEdit->text())
            .setEvaluation(ui->evaluationSlider->value())
            .setTimeStamp(timeStamp);

        QFile MP3File(Config::getInstance().tempMP3FileName());
        bool moved = MP3File.rename(config.outboxDirectory().absoluteFilePath(fileNamePrefix + ".mp3"));
        if(!moved) {
            qCritical() << "Couldn't move MP3 file to outbox directory : " + config.outboxDirectory().absolutePath();
            QMessageBox::critical(mainWindow, "Erreur", "L'enregistrement du message dans la base de données a échoué, nous en sommes désolés.");
            stateMachine->submitEvent("error");
            return;
        }

        try {
            jsonFile.save();
            qDebug() << "Json file has been saved correctly.";
            QMessageBox::information(mainWindow, "Information", "Votre message a été enregistré.");
            mainWindow->audioRecorder.clear();
            stateMachine->submitEvent("saved");
        } catch (QString s) {
            qCritical() << "Could'nt create json file in outbox directory :";
            qCritical() << "Caught exception ! " + s;
            qDebug() << "Trying to delete MP3 file in outbox directory (" + QFileInfo(MP3File).absoluteFilePath() + ")";
            if(MP3File.remove() == true)
                qDebug() << "    MP3 file removed with success.";
            else
                qDebug() << "    Failed to delete MP3 file.";
            QMessageBox msgBox;
            msgBox.setText("L'enregistrement du message dans la base de données a échoué, nous en sommes désolés.");
            msgBox.exec();
            stateMachine->submitEvent("error");
        }
    }
    else {
        qDebug() << "Exiting saveMessageState";
    }
}

void StateHandler::savedMessageState(bool active) {
    if(active) {
        qDebug() << "Entering savedMessageState";
        ui->stackedWidget->setCurrentIndex(5);
        QTimer::singleShot(10000, [this]() { stateMachine->submitEvent("home"); });
    }
    else {
        qDebug() << "Exiting savedMessageState";
    }
}

void StateHandler::listeningMessageState2(bool active) {
    if (active) {
        qDebug("Entering listeningMessageState2");
        if(!ui->tableView->selectionModel()->hasSelection()) {
            qDebug() << "Please select a message to play.";
            stateMachine->submitEvent("stop");
            return;
        }
        mainWindow->mediaPlayList.clear();
        auto p = ui->tableView->selectionModel()->selectedRows();
        QListIterator<QModelIndex> i(p);
        while(i.hasNext()) {
            QModelIndex index = i.next();
            QString fileName = mainWindow->proxyModel.data(index.siblingAtColumn(MP3FILENAME_COLUMN)).toString();
            qDebug() << mainWindow->proxyModel.data(index.siblingAtColumn(MP3FILENAME_COLUMN)).toString();
            mainWindow->mediaPlayList.addMedia(QUrl::fromLocalFile(Config::getInstance().inboxDirectory().absoluteFilePath(fileName)));
        }
        mainWindow->mediaPlayer.setPlaylist(&mainWindow->mediaPlayList);
        mainWindow->mediaPlayer.play();
    }
    else {
        mainWindow->mediaPlayer.stop();
        qDebug() << "Exiting listeningMessageState2";
    }

}

void StateHandler::waitForPlayerState(bool active) {
    if(active) {
        qDebug() << "Entering waitForPlayerState";
    }
    else {
        qDebug() << "Exiting waitForPlayerState";
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
