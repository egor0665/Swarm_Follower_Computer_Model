#include "serialporthandler.h"
#include <QDebug>
#include <string>

SerialPortHandler::SerialPortHandler(int targetPort, int baudRate)
{
    initialize_defaults();
    this->targetPort = "COM" + QString::number(targetPort);
    this->baudRate = baudRate;
    serialPort = new QSerialPort();
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readMessage()));
    openSerialPort();
}

void SerialPortHandler::initialize_defaults()
{
    // Initialize attributes
    targetPort = "COM1";
    baudRate = QSerialPort::Baud9600;
    isOpen = false;
}


SerialPortHandler::~SerialPortHandler()
{
    delete(serialPort);
}

// =================================================================================================================================================================================================================
// =========================================================================================== OPEN CONNECTION =====================================================================================================
// =================================================================================================================================================================================================================

void SerialPortHandler::openSerialPort()
{
    serialPort->setPortName(targetPort);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite))
    {
        qDebug()<< "serial opened";
//        this->start();
    }
    else
    {
        qDebug()<< "serial error";
        serialPort->error();
    }
}

bool SerialPortHandler::_openPort()
{
    serialPort->open(QIODevice::ReadWrite);
    return serialPort->isOpen();
}


// =================================================================================================================================================================================================================
// =========================================================================================== READER / WRITER =====================================================================================================
// =================================================================================================================================================================================================================


void SerialPortHandler::readMessage()
{
    QByteArray data = _readPort();
    qDebug()<<data;
    theData.append(data);
    if (theData[theData.length()-1]=='\n'){
        theData.remove(theData.length(),1);
        emit messageReceived(theData);
        theData.clear();
    }

    return;
}

QByteArray SerialPortHandler::_readPort()
{
    return serialPort->readAll();
}

int SerialPortHandler::_writePort(char *data, uint16_t len)
{
    //qDebug()<<len;
//    QVector<int> msg;
//    for (int i=0;i<len;i++)
//        msg.append(data[i]);
    //qDebug()<< "sending message "<< msg;
    int resBytesWritten = serialPort->write(data,len);
    //qDebug()<<resBytesWritten;
    return resBytesWritten;
}


bool SerialPortHandler::start()
{
    isOpen = _openPort();
    return isOpen;
}

bool SerialPortHandler::stop()
{
    serialPort->close();

    isOpen = false;
    return isOpen;
}
