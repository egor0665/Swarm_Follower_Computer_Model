#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int displayWidgetHeight = 600;
    int displayWidgetWidth = 1600;
    displayWidget.setMinimumSize(displayWidgetWidth,displayWidgetHeight);
    ui->verticalLayout->addWidget(&displayWidget);
    serialPortHandler = new SerialPortHandler(3,115200);
    connect(serialPortHandler,&SerialPortHandler::messageReceived, this, &MainWindow::handleMessage);
}

void MainWindow::handleMessage(QByteArray data)
{
    Position* position = reinterpret_cast<Position*>(data.data());
    qDebug()<<data.size();
    qDebug()<<position->x<<position->y<<position->z;
    displayWidget.setCurrentSignalPosition(*position);
}

MainWindow::~MainWindow()
{
    delete ui;
}

