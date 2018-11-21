#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#define TIMER_TIMEOUT (500)
static const uchar cDATA_CURE[15]={0x5a,0x30,0x02,0x08,0x06,0x06,0x06,0x12,0x05,0x10,0x10,0x01,0x30,0x10,0xa5};
static const uchar cDATA_REST[15]={0x5a,0x30,0x02,0x08,0x06,0x06,0x06,0x12,0x05,0x10,0x10,0x00,0x30,0x10,0xa5};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isFesRuning=false;
    DATA_REST.setRawData((char*)cDATA_REST,15);
    DATA_CURE.setRawData((char*)cDATA_CURE,15);

   // DATA_REST.fromRawData(cDATA_REST,sizeof(cDATA_REST));
   // DATA_CURE.fromRawData(cDATA_CURE,sizeof(cDATA_CURE));
    //serialPort = 0;
    //寻找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        if (ui->comboBox_port->currentText() != info.portName())
            ui->comboBox_port->addItem(info.portName());
    }
    QObject::connect(&serialPort, SIGNAL(readyRead()), this, SLOT(ReadData()));//发来数据就读取
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

MainWindow::~MainWindow()
{
    if(serialPort.isOpen()){
        serialPort.close();
    }
    delete ui;
}

void MainWindow::on_timeout()
{
    autoSimu();
}

void MainWindow::on_pushButton_link_clicked()
{
    if(serialPort.isOpen()){
        serialPort.close();
        ui->pushButton_link->setText("连接");
    }
    else{
    ui->pushButton_link->setText("断开");
    serialPort.setPortName(ui->comboBox_port->currentText());//选取串口
    serialPort.open(QIODevice::ReadWrite);             //打开串口
    serialPort.setBaudRate(ui->comboBox_2_baudrate->currentText().toInt());
    //qDebug() << ui.BaudRate->currentText().toInt() << endl;
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    }

}

void MainWindow::ReadData()
{
    if(ui->textEdit_in->isFullScreen()){
       ui->textEdit_in->clear();
    }
    requestData = serialPort.readAll();          //字符型
    QString srequestData;
    srequestData=requestData.toHex(' ');
    if(srequestData=="5a 0b 00 a5"){
        isFesRuning=true;
    }
    else if(srequestData=="5a 0c 00 a5"){
        isFesRuning=false;
    }
    ui->textEdit_in->append(srequestData);
}
void MainWindow::dataSend()
{
    QString data = ui->textEdit_out->toPlainText();
    QByteArray DATA;
    DATA = data.toLatin1();
    serialPort.write(DATA);
    ReadData();
}
void MainWindow::autoSimu()
{
    //serialPort.clear(QSerialPort::Output);
    if(isFesRuning==true)
    {
        serialPort.write(DATA_CURE);
        qDebug() <<"DATA_CURE";
    }
    else{
        serialPort.write(DATA_REST);
        qDebug() <<"DATA_REST";
    }
}

void MainWindow::Exit()
{

    close();
}

void MainWindow::on_pushButton_4_send_clicked()
{

}

void MainWindow::on_checkBox_3_autosimu_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
    {
        timer.start(500);
    }
    else if(arg1==Qt::Unchecked)
    {
        timer.stop();

    }

}
void MainWindow::StringToHex(QString str, QByteArray & senddata)  //字符串转换成16进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    //        else return (-1);
    else return ch-ch;//不在0-f范围内的会发送成0
}

void MainWindow::on_pushButton_2_cureing_clicked()
{
    serialPort.write(DATA_CURE);

}

void MainWindow::on_pushButton_3_resting_clicked()
{
    serialPort.write(DATA_REST);
}
