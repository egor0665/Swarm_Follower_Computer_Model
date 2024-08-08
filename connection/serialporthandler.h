#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <windows.h>
#include <QtSerialPort/QSerialPort>
#include "genericport.h"


class SerialPortHandler: public GenericPort
{
    Q_OBJECT

    QByteArray theData;

    QSerialPort *serialPort;
    QString targetPort;
    int baudRate;
    bool isOpen;

    bool is_running(){
        return isOpen;
    }
    bool start();
    bool stop();

public:
    SerialPortHandler(int targetPort, int baudRate);
    ~SerialPortHandler();
    void openSerialPort();

protected:
    void initialize_defaults();
    bool _openPort();
    QByteArray _readPort();
    int _writePort(char *data, uint16_t len);

private slots:
    void readMessage();
signals:
    void messageReceived(QByteArray data);
};

#endif // SERIALPORTHANDLER_H
