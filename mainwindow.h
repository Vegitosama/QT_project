#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QDateTime>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButtonDisconnect_clicked();

    void readFromPort();

    void on_Temp_clicked();

    void on_Hum_clicked();

private:
    Ui::MainWindow *ui;

    void addToLogs(QString message);

    QSerialPort *device;

    void sendMessageToDevice(QString message);


};
#endif // MAINWINDOW_H
