#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonSearch_clicked()
{
    this->addToLogs("Szukam urządzeń...");
      QList<QSerialPortInfo> devices;
      devices = QSerialPortInfo::availablePorts();

      for(int i = 0; i < devices.count(); i++) {
        this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
        ui->comboBoxDevices->addItem(devices.at(i).portName() + "\t" + devices.at(i).description());
      }
    }

void MainWindow::on_pushButtonConnect_clicked()
{

    if(ui->comboBoxDevices->count() == 0) {
      this->addToLogs("Nie wykryto żadnych urządzeń!");
      return;
    }

    QString portName = ui->comboBoxDevices->currentText().split("\t").first();
    this->device->setPortName(portName);



    if(!device->isOpen()) {
       // OTWÓRZ I SKONFIGURUJ PORT:
    if(device->open(QSerialPort::ReadWrite)) {
      this->device->setBaudRate(QSerialPort::Baud9600);
      this->device->setDataBits(QSerialPort::Data8);
      this->device->setParity(QSerialPort::NoParity);
      this->device->setStopBits(QSerialPort::OneStop);
      this->device->setFlowControl(QSerialPort::NoFlowControl);


      this->addToLogs("Otwarto port szeregowy.");
    } else {
      this->addToLogs("Otwarcie porty szeregowego się nie powiodło!");

    }
    // CONNECT:
    connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
      }
    else {
      this->addToLogs("Port już jest otwarty!");
      return;
    }
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}

void MainWindow::sendMessageToDevice(QString message)
{
    if(this->device->isOpen() && this->device->isWritable()) {
       this->addToLogs("Wysyłam informacje do urządzenia " + message);
        this->device-> write(message.toStdString().c_str());
      } else {
        this->addToLogs("Nie mogę wysłać wiadomości. Port nie jest otwarty!");
      }
}


void MainWindow::on_pushButtonDisconnect_clicked()
{

    if(this->device->isOpen()) {
      this->device->close();
      this->addToLogs("Zamknięto połączenie.");
    } else {
      this->addToLogs("Port nie jest otwarty!");
      return;
    }
}

void MainWindow::readFromPort()
{
    while(this->device->canReadLine()) {
        QString line = this->device->readLine();
        //qDebug() << line;

        QString terminator = "\r";
        int pos = line.lastIndexOf(terminator);
        //qDebug() << line.left(pos);

        this->addToLogs(line.left(pos));
      }
}

void MainWindow::on_Temp_clicked()
{
    this->sendMessageToDevice("GET TEMP\n");

}

void MainWindow::on_Hum_clicked()
{
   this->sendMessageToDevice("GET HUM\n");
}
