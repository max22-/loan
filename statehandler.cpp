#include <QMessageBox>
#include "statehandler.h"

StateHandler::StateHandler(Ui::MainWindow *ui, Statechart *stateMachine, QObject *parent) : QObject(parent)
{
    this->ui = ui;
    this->stateMachine = stateMachine;
}

void StateHandler::homeState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}


void StateHandler::tocState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void StateHandler::recordFormState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void StateHandler::listenHomeState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(8);
    }
}

void StateHandler::validateFormState(bool active) {
    if(active) {
        QString nickname = ui->nicknameLineEdit->text();
        QString age = ui->ageLineEdit->text();
        QString city = ui->cityLineEdit->text();
        int pos = 0;
        QRegExpValidator *nicknameValidator = new QRegExpValidator(QRegExp("^[a-z0-9_-]{3,15}$"), nullptr);
        QIntValidator *ageValidator = new QIntValidator(0, 200);
        QRegExpValidator *cityValidator = new QRegExpValidator(QRegExp("^[a-z-]$"), nullptr);
        QMessageBox msgBox;
        if(nicknameValidator->validate(nickname, pos) != QValidator::Acceptable) {
            msgBox.setWindowTitle("Erreur de saisie");
            msgBox.setText("Pseudonyme invalide.");
            msgBox.exec();
            stateMachine->submitEvent("notOk");
        }
        delete nicknameValidator;
        delete ageValidator;
        delete cityValidator;
    }
}

void StateHandler::DataEnteredState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(3);
    }
}
