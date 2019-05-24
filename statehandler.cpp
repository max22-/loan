#include "statehandler.h"

StateHandler::StateHandler(Ui::MainWindow *ui, QObject *parent) : QObject(parent)
{
    this->ui = ui;
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

void StateHandler::recordHomeState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void StateHandler::listenHomeState(bool active) {
    if(active) {
        ui->stackedWidget->setCurrentIndex(8);
    }
}
