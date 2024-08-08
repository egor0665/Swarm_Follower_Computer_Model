#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DisplayWidget.h>
#include <QMainWindow>

#include <connection/serialporthandler.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    DisplayWidget displayWidget;
    SerialPortHandler *serialPortHandler;

    Position* sumPosition;
    int sumCount;
    Position resultSumPosition;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void handleMessage(QByteArray data);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
