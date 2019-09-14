#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include<QUuid>
#include <QIntValidator>
#include <QRegExpValidator>
#include"socketdata.h"
namespace Ui {
class socketclient;
}

class socketclient : public QMainWindow
{
    Q_OBJECT

public:
    explicit socketclient(QWidget *parent = nullptr);
    ~socketclient();

public slots:
    void connectServer();
    void disconnectServer();
    void readMesg();
    void sendMesg();
    void regulateInput();
    void Init();
private:
    Ui::socketclient *ui;
    socketData* data;
    QTcpSocket* m_tcpSocket;
    QUuid this_id;
};

#endif // SOCKETCLIENT_H
