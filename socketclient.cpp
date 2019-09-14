#include "socketclient.h"
#include "ui_socketclient.h"

socketclient::socketclient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::socketclient)
{
    ui->setupUi(this);
    this_id = QUuid::createUuid();
    connect(ui->pushButton_connect,SIGNAL(clicked()),this,SLOT(connectServer()));
    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(sendMesg()));
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(sendMesg()));
    Init();
}

void socketclient::Init(){
    m_tcpSocket = new QTcpSocket(this);
    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit_port->setText("9000");
    data = new socketData(m_tcpSocket);
    regulateInput();
}

void socketclient::regulateInput(){
    ui->lineEdit_port->setValidator(new QIntValidator(0,65535,this));
    // ip地址输入
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    ui->lineEdit_ip->setValidator(new QRegExpValidator(rx, this));
}

socketclient::~socketclient()
{
    delete ui;
}

void socketclient::connectServer()
{
    QHostAddress addr = QHostAddress(ui->lineEdit_ip->text());
    quint16 port = ui->lineEdit_port->text().toUShort();
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_connect->repaint();
    if(m_tcpSocket == nullptr){
       m_tcpSocket = new QTcpSocket(this);
       m_tcpSocket->connectToHost(addr,port);
   }
   else{
       m_tcpSocket->connectToHost(addr,port);
   }
    m_tcpSocket->waitForConnected(3000);
    if(m_tcpSocket->state() == QAbstractSocket::ConnectedState){
        connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMesg()));
        ui->pushButton_connect->setText("Disconnect");
        disconnect(ui->pushButton_connect,SIGNAL(clicked()),this,SLOT(connectServer()));
        connect(ui->pushButton_connect,SIGNAL(clicked()),this,SLOT(disconnectServer()));
    }
    ui->pushButton_connect->setEnabled(true);

}

void socketclient::disconnectServer(){
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_connect->repaint();
    if(m_tcpSocket != nullptr){
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->waitForDisconnected(3000);
        if(m_tcpSocket->state() == QAbstractSocket::UnconnectedState){
            disconnect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMesg()));
            ui->pushButton_connect->setText("Connect");
            disconnect(ui->pushButton_connect,SIGNAL(clicked()),this,SLOT(disconnectServer()));
            connect(ui->pushButton_connect,SIGNAL(clicked()),this,SLOT(connectServer()));
        }
    }
    ui->pushButton_connect->setEnabled(true);

}

void socketclient::readMesg() //读取信息
{
  QByteArray qba =   m_tcpSocket->readAll();

//  ui->textEdit_recmesg->clear();

  data->appendChatData(QString(qba),nullptr,&this_id);
  ui->textBrowser->clear();
  ui->textBrowser->moveCursor(QTextCursor::End);
  ui->textBrowser->insertHtml(data->getData(&this_id, "对方"));
  ui->textBrowser->moveCursor(QTextCursor::End);

  QString ss=QVariant(qba).toString();
//  ui->textEdit_recmesg->setText(ss);
}

void socketclient::sendMesg() //发送信息
{
   QString ss= ui->lineEdit->text();
   if(ss.length()){
       if(m_tcpSocket->state() == 3){
           m_tcpSocket->write(ss.toStdString().c_str(),strlen(ss.toStdString().c_str()));
           ui->lineEdit->clear();
           data->appendChatData(ss,&this_id,nullptr);
           ui->textBrowser->clear();
           ui->textBrowser->moveCursor(QTextCursor::End);
           ui->textBrowser->insertHtml(data->getData(&this_id, "对方"));
           ui->textBrowser->moveCursor(QTextCursor::End);

       }
   }
}
