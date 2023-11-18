#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include "curve.h"
#include "kuznechik.h"

class Client;

class ClientDestroyer
{
private:
    Client * pInstance;
public:
    /**
         * @brief clientDestroyer::~clientDestroyer деструктор
         */
    ~ClientDestroyer();
    /**
        * @brief clientDestroyer::initialize копирует ссылку на объект класса client
         * для последующей работы с ней в классе clientDestroyer
        * @param p
        */
    void initialize(Client * p);
};


class Client: public QObject
{
    Q_OBJECT
private:
    static Client * pInstance;
    static ClientDestroyer destroyer;
    static QTcpSocket * mTcpSocket;
    //general info
    QString login;
    QString openKey;
    QString userStatus;
    QString password;
    uint1024_t privateKey;
    QString serverStatus;
    //info for communication with server
    uint1024_t myServerShortTermPrivateKey;
    Point serverOpenKey;
    QString serverKey;
    QString serverIV;

protected:
    Client(QObject *parent = nullptr);
    ~Client();

    Client(const Client& );
    Client& operator = (Client &);
    friend class ClientDestroyer;

public:
    static Client* getInstance();
    void sendToServer(QString message);

    //set general info
    void setOpenKey(QString openKey);
    void setPrivateKey(uint1024_t privateKey);
    void setLogin(QString login);
    void setServerKey(QString key);
    void setServerIV(QString IV);
    void setPassword(QString password);
    void setUserStatus(QString status);
    void setMyServerShortTermPrivateKey(uint1024_t privateKey);
    //get general info
    uint1024_t getPrivateKey();
    QString getLogin();
    QString getUserStatus();
    QString getServerStatus();
    QString getServerKey();
    QString getServerIV();
    QString getPassword();
    uint1024_t getMyServerShortTermPrivateKey();

    void resetPersonalData();
    void disconnect();

signals:
    void receivedMyOpenKey(Point openKeyPoint);
    void receivedServerShortTermOpenKey(Point longTermOpenKey, Point shortTermOpenKey);
    void receivedUserOpenKey(Point openKeyPoint, QString login);
    void receivedShortTermOpenKey(QString login, Point longTermOpenKey, Point shortTermOpenKey);
    void receivedAuthResult(QString status, QString userStatus);

public slots:
    void slotServerRead();
};


#endif // CLIENT_H
