#include "mytcpserver.h"

Curve * Curve::pInstance=0;
CurveDestroyer Curve::destroyer;
int Curve::a;
int Curve::b;
uint256_t Curve::p;
Point Curve::G;
Point Curve::O;

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

/*конструктор для запуска сервера*/
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "SKachKi server is not started";
    } else {
        //server_status=1;
        qDebug() << "SKachKi server is started";
    }
}


/*создание слота для нового подключения*/
void MyTcpServer::slotNewConnection()
{
    QTcpSocket* mTcpSocket = mTcpServer->nextPendingConnection();
    list.push_back(mTcpSocket);

    qDebug() << mTcpSocket;
        //mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
    connect(mTcpSocket, &QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
    connect(mTcpSocket,&QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);

}

/*чтение строки от пользователя*/
void MyTcpServer::slotServerRead()
{
    Functions f;
    QTcpSocket* mTcpSocket = (QTcpSocket*)sender();
    QString res = "";
    while (mTcpSocket->bytesAvailable()>0) {
        QByteArray array = mTcpSocket->readAll();
        res.append(array);
    }
    mTcpSocket->write(f.parse(res).toUtf8());

}

/*возможность подключения нескольких клиентов*/
void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* mTcpSocket = (QTcpSocket*)sender();
    list.removeAt(list.indexOf(mTcpSocket));
    mTcpSocket->close();
    //qDebug() << list.size();
}
