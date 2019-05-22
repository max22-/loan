#include "statehandler.h"

StateHandler::StateHandler(Ui::MainWindow *ui, QObject *parent) : QObject(parent)
{
    this->ui = ui;
}

void StateHandler::tocState(bool active) {
    if(active) {
        qDebug("TOC!");
        ui->stackedWidget->setCurrentIndex(1);
    }
}
