#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [this]() { this->stateMachine.submitEvent("toc"); });
    stateMachine.connectToState("TocState", &stateHandler, SLOT(tocState(bool)));
    stateMachine.start();

    //stateMachine.submitEvent("toc");
}

MainWindow::~MainWindow()
{
    delete ui;
}
