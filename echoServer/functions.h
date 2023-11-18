#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <QRegularExpression>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTcpSocket>
#include <QMap>
#include "curve.h"
#include "database.h"
#include "kuznechik.h"
#include "streebog.h"

class Functions
{
private:
    QSqlDatabase *db;
    Curve *secp256k1;
    Point shortTermOpenKey;
    QString sendOpenKey(QString login);
    QString sendShortTermOpenKey(QString login);
    void generateShortTermKey(uint1024_t &shortTermPrivateKey);
    Point mqv(Point longTermOpenKey, Point shortTermOpenKey);


public:
    Functions();
    QString parse(QString dataFromClient, QMap<QTcpSocket*, QVector<QString>> &sockets, QTcpSocket* socket);
    QString convertTextToHex(QString text);
    QString checkPassword(QString login, QString password);

};

#endif // FUNCTIONS_H
