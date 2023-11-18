#include "client.h"
#include <QDebug>
#include <QCoreApplication>

ClientDestroyer::~ClientDestroyer(){
    delete pInstance;
}

void ClientDestroyer::initialize(Client * p){
    pInstance = p;
}

Client::Client(QObject *parent){
    mTcpSocket = new QTcpSocket(this);
    mTcpSocket->connectToHost("127.0.0.1", 33333);
    if(mTcpSocket->waitForConnected() ){
        QString num = "51739109251054544900652827515386315264922377181505051896146948101942003159361";
        this->serverOpenKey.x = (uint256_t)num.toStdString();
        num = "12864400938217234001090395423358871809299620525603885367207698537354801609454";
        this->serverKey = "4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048";
        this->serverIV = "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea";
        this->serverOpenKey.y = (uint256_t)num.toStdString();
        this->serverStatus = "active";
        this->userStatus = "anonymous";
    }
    else{
        this->serverStatus = "inactive";

        disconnect();
    }
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &Client::slotServerRead);
}

Client::~Client(){
    mTcpSocket->close();
}

Client* Client::getInstance(){
    if (!pInstance)
    {
        pInstance = new Client();
        destroyer.initialize(pInstance);
    }
    return pInstance;
}

void Client::disconnect()
{
    this->~Client();
}

void Client::sendToServer(QString message){
    //qDebug() << "Send to server" << message;
    message = encryptKuznechikOFB(message.toUtf8().toHex(), this->serverKey, this->serverIV);
    //qDebug() << "Send to server" << message;
    mTcpSocket->write(message.toUtf8());
    //mTcpSocket->write(message.toUtf8());
}

 //set general info

void Client::setOpenKey(QString openKey)
{
    this->openKey = openKey;
}

void Client::setPrivateKey(uint1024_t privateKey)
{
    this->privateKey = privateKey;
}

void Client::setLogin(QString login)
{
    this->login = login;
}

void Client::setServerKey(QString key)
{
    this->serverKey = key;
}

void Client::setServerIV(QString IV)
{
    this->serverIV = IV;
}

void Client::setUserStatus(QString status)
{
    this->userStatus = status;
}


void Client::setMyServerShortTermPrivateKey(uint1024_t privateKey)
{
    this->myServerShortTermPrivateKey = privateKey;
}

void Client::setPassword(QString password)
{
    this->password = password;
}

 //get general info

uint1024_t Client::getPrivateKey()
{
    return this->privateKey;
}

QString Client::getLogin()
{
    return this->login;
}

QString Client::getUserStatus()
{
    return this->userStatus;
}

QString Client::getServerStatus()
{
    return this->serverStatus;
}

QString Client::getServerKey()
{
    return this->serverKey;
}

QString Client::getServerIV()
{
    return this->serverIV;
}

QString Client::getPassword()
{
    return this->password;
}

uint1024_t Client::getMyServerShortTermPrivateKey()
{
    return this->myServerShortTermPrivateKey;
}

void Client::resetPersonalData()
{
    this->login = "";
    this->password = "";
    this->privateKey = 0;
    this->userStatus = "anonymous";
    this->serverKey = "4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048";
    this->serverIV = "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea";

}


void Client::slotServerRead(){
    QString array = "";
    while(mTcpSocket->bytesAvailable()>0)
    {
        QByteArray res = mTcpSocket->readAll();
        array.append(res);

        //qDebug() << "slotServerRead before";
        //qDebug() << array;
        array = decryptKuznechikOFB(array, this->serverKey, this->serverIV);
        QByteArray hex = QByteArray::fromHex(array.toUtf8());
        array = hex.data();

        //qDebug() << "slotServerRead after";
        //qDebug() << array;
        QStringList list = array.split("&", Qt::SkipEmptyParts);
        if(list[0] == "openKey"){
            //qDebug() << "sir";
            Point openKeyPoint;
            openKeyPoint.x = (uint256_t)list[2].toStdString();
            openKeyPoint.y = (uint256_t)list[3].toStdString();
            if(list[1] == this->login){
                //qDebug() << "1";
                emit receivedMyOpenKey(openKeyPoint);
                //qDebug() << "send";
            }
            else{
                emit receivedUserOpenKey(openKeyPoint, login);
            }
        }
        else if(list[0] == "shortTermOpenKey"){
            if(list[2] == this->login){
                QString from;
                Point longTermOpenKey;
                Point shortTermOpenKey;
                shortTermOpenKey.x = (uint256_t)list[3].toStdString();
                shortTermOpenKey.y = (uint256_t)list[4].toStdString();
                if(list[1] == "Server"){
                    longTermOpenKey = this->serverOpenKey;
                    emit receivedServerShortTermOpenKey(longTermOpenKey, shortTermOpenKey);
                }
                else {
                    emit receivedShortTermOpenKey(list[1], longTermOpenKey, shortTermOpenKey);
                }

            }
        }
        else if(list[0] == "auth"){
            emit receivedAuthResult(list[1], list[2]);
        }
    }

}
