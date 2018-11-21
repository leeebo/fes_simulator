#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_link_clicked();
    void ReadData();
    void dataSend();
    void autoSimu();
    void Exit();
    void on_timeout();
    void on_pushButton_4_send_clicked();
    void on_checkBox_3_autosimu_stateChanged(int arg1);

    void on_pushButton_2_cureing_clicked();

    void on_pushButton_3_resting_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    QTimer timer;
    QByteArray requestData;
    QByteArray DATA_CURE;
    QByteArray DATA_REST;
    volatile bool isFesRuning;
    void StringToHex(QString str, QByteArray & senddata);
    char ConvertHexChar(char ch);
};

#endif // MAINWINDOW_H
