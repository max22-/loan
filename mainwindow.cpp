#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    stateHandler = new StateHandler(ui);

    connectButton(ui->tocButton, "toc");
    connectButton(ui->recordHomeButton, "record");
    connectButton(ui->listenHomeButton, "listen");
    connectButton(ui->backButton1, "back");
    connectButton(ui->backButton2, "back");

    connectState("HomeState", &StateHandler::homeState);
    connectState("TocState", &StateHandler::tocState);
    connectState("RecordHomeState", &StateHandler::recordHomeState);
    connectState("ListenHomeState", &StateHandler::listenHomeState);

    stateMachine.start();

}

MainWindow::~MainWindow()
{
    delete stateHandler;
    delete ui;
}

void MainWindow::connectButton(const QPushButton *button, const char *eventName) {
    connect(button, &QPushButton::clicked, [this, eventName]() { this->stateMachine.submitEvent(eventName); });
}

void MainWindow::connectState(const char *stateName, void (StateHandler::*stateHandlerMember)(bool) ) {
    stateMachine.connectToState(stateName, stateHandler, stateHandlerMember);
}
