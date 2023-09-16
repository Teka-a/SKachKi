#include "functions.h"

Functions::Functions()
{
    Database *db = Database::getInstance();
    Curve *secp256k1 = Curve::getInstance();
}

QString Functions::sendOpenKey(QString login)
{

    QString responce = "openKey&" + login + "&";

    QSqlQuery query;
    query.prepare("SELECT * FROM users "
                  "WHERE login == :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int keyY = rec.indexOf("open_key_y");
    const int keyX = rec.indexOf("open_key_x");

    while(query.next()){
        qDebug() << "in next";
        responce += query.value(keyX).toString() + "&" + query.value(keyY).toString() + "&";
        qDebug() << responce;
    }

    return responce;

}

void Functions::generateShortTermKey(long long &shortTermPrivateKey)
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    qDebug() << partsUuid[3];
    bool ok;
    shortTermPrivateKey = partsUuid[3].toLongLong(&ok, 16);
    qDebug() << shortTermPrivateKey;
    this->shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
}

QString Functions::sendShortTermOpenKey(QString login)
{
    QString responce = "shortTermOpenKey&Server&" + login + "&";

    responce += QString::fromStdString(this->shortTermOpenKey.x.str()) + "&" + QString::fromStdString(this->shortTermOpenKey.y.str()) + "&";
    return responce;
}

void Functions::mqv(Point longTermOpenKey, Point shortTermOpenKey)
{
    qDebug() << "MQV stage 1";

    long long kA;
    generateShortTermKey(kA);

    Point rA = this->shortTermOpenKey;
    Point qA = secp256k1->countComposition((long long)83666);

    uint256_t xA = shortTermOpenKey.x;
    uint256_t aA = longTermOpenKey.x;
    uint256_t dA = 83666;

    uint1024_t sA = xA*aA*dA;
    sA += kA;
    sA %= (uint1024_t)secp256k1->getN().toStdString();
    qDebug() << QString::fromStdString(sA.str());
    //s %= (uint1024_t)secp256k1->getN();


    qDebug() << "MQV stage 2";
    long long xB = QString::fromStdString(shortTermOpenKey.x.str()).toLongLong();
    long long aB = QString::fromStdString(longTermOpenKey.x.str()).toLongLong();

    Point U = secp256k1->countComposition(aB, longTermOpenKey);
    U = secp256k1->countComposition(xB, U);
    U = secp256k1->addPoint(shortTermOpenKey, U);
    qDebug() << QString::fromStdString(U.x.str()) << QString::fromStdString(U.y.str());

    qDebug() << "MQV stage 3";
    Point W = secp256k1->countComposition(QString::fromStdString(sA.str()).toLongLong(), U);
    qDebug() << QString::fromStdString(W.x.str()) << QString::fromStdString(W.y.str());
}


QString Functions::parse(QString dataFromClient)
{
    qDebug() << "data client" << dataFromClient;

    QStringList list = dataFromClient.split("&", Qt::SkipEmptyParts);

    if(list[0] == "requestOpenKey"){
        //requestOpenKey&user1
        return sendOpenKey(list[1]);
    }
    else if(list[0] == "shortTermOpenKey"){
        if(list[2] == "Server"){
            QStringList keyInfo = sendOpenKey(list[1]).split("&", Qt::SkipEmptyParts);
            Point longTermOpenKey;
            Point shortTermOpenKey;
            longTermOpenKey.x = (uint256_t)keyInfo[2].toStdString();
            longTermOpenKey.y = (uint256_t)keyInfo[3].toStdString();

            shortTermOpenKey.x = (uint256_t)list[3].toStdString();
            shortTermOpenKey.y = (uint256_t)list[4].toStdString();
            mqv(longTermOpenKey, shortTermOpenKey);
            return sendShortTermOpenKey(list[1]);
        }

    }

    return "get it!";

}
