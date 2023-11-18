#include "functions.h"

Functions::Functions()
{
    Database *db = Database::getInstance();
    Curve *secp256k1 = Curve::getInstance();
}

QString Functions::checkPassword(QString login, QString password)
{
    QString responce = "auth&";
    QSqlQuery query;
    query.prepare("SELECT password FROM users "
                  "WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int key = rec.indexOf("password");

    QString hashDB = "";
    while(query.next()){
        hashDB = query.value(key).toString();

    }
    password = password.toUtf8().toHex();
    QString hash = getHash512(password);
    //qDebug() << hash;
    //qDebug() << hashDB;
    if (hash == hashDB) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    //add status
    query.prepare("SELECT status.name, users.login FROM users "
                  "INNER JOIN status ON users.status_id = status.id "
                  "WHERE users.login = :login");
    query.bindValue(":login", login);
    query.exec();
    //qDebug() << query.exec();
    //qDebug() << query.lastError();
    QString userStatus = "";

    while(query.next()){
        userStatus = query.value(0).toString();
    }
    //qDebug() << userStatus;
    responce += "&" + userStatus;
    responce = responce.simplified();
    responce.replace( " ", "" );

    //qDebug() << "checkPass" << responce;
    return responce;
}

QString Functions::sendOpenKey(QString login)
{

    QString responce = "openKey&" + login + "&";

    QSqlQuery query;
    query.prepare("SELECT ltok FROM users "
                  "WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int key = rec.indexOf("ltok");

    while(query.next()){
        //qDebug() << query.value(0);
        responce += query.value(key).toString();
        //qDebug() << responce;
    }
    //qDebug() << "openKey" << responce;
    return responce;
}

void Functions::generateShortTermKey(uint1024_t &shortTermPrivateKey)
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    //qDebug() << partsUuid[3];
    bool ok;
    shortTermPrivateKey = partsUuid[3].toLongLong(&ok, 16);
    //qDebug() << QString::fromStdString(shortTermPrivateKey.str());
    this->shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
}

QString Functions::sendShortTermOpenKey(QString login)
{
    //зачем логин?
    QString responce = "shortTermOpenKey&Server&" + login + "&";

    responce += QString::fromStdString(this->shortTermOpenKey.x.str()) + "&" + QString::fromStdString(this->shortTermOpenKey.y.str()) + "&";
    return responce;
}

Point Functions::mqv(Point longTermOpenKey, Point shortTermOpenKey)
{
    //Server's privates keys
    uint1024_t dA = 83666;
    uint1024_t kA;
    generateShortTermKey(kA);
    //Server's open keys
    //shortTerm
    Point rA = this->shortTermOpenKey;
    //qDebug() << "Server short open: " << QString::fromStdString(rA.x.str()) << " " << QString::fromStdString(rA.y.str());
    uint1024_t xA = (uint1024_t)rA.x;
    //longTerm
    Point qA = secp256k1->countComposition(83666);
    //qDebug() << "Server long open: " << QString::fromStdString(qA.x.str()) << " " << QString::fromStdString(qA.y.str());
    uint256_t aA = qA.x;
    //Client's open keys
    //shortTerm
    Point rB = shortTermOpenKey;
    //qDebug() << "Client short open: " << QString::fromStdString(rB.x.str()) << " " << QString::fromStdString(rB.y.str());
    uint1024_t xB = (uint1024_t)rB.x;
    //longTerm
    Point qB = longTermOpenKey;
    //qDebug() << "Client long open: " << QString::fromStdString(qB.x.str()) << " " << QString::fromStdString(qB.y.str());
    uint1024_t aB = (uint1024_t)qB.x;


    //qDebug() << "MQV stage 1";

    uint1024_t sA = xA*aA*dA;
    sA += kA;
    sA %= (uint1024_t)secp256k1->getN().toStdString();
    //qDebug() << "sA =" << QString::fromStdString(sA.str());


    //qDebug() << "MQV stage 2";


    Point uA = secp256k1->countComposition(aB, qB);
    uA = secp256k1->countComposition(xB, uA);
    uA = secp256k1->addPoint(rB, uA);
    //qDebug() << "uA" << QString::fromStdString(uA.x.str()) << QString::fromStdString(uA.y.str());

    //qDebug() << "MQV stage 3";
    Point W = secp256k1->countComposition(sA, uA);
    //qDebug() << "W" << QString::fromStdString(W.x.str()) << QString::fromStdString(W.y.str());
    return W;

}


QString Functions::parse(QString dataFromClient, QMap<QTcpSocket*, QVector<QString>> &sockets, QTcpSocket* socket)
{
    qDebug() << "data client" << dataFromClient;

    QStringList list = dataFromClient.split("&", Qt::SkipEmptyParts);
    //requestOpenKey&userLogin&
    if(list[0] == "requestOpenKey"){
        //requestOpenKey&user1
        return sendOpenKey(list[1]);
    }
    //shortTermOpenKey&userLogin&Server&stok.x&stok.y&
    else if(list[0] == "shortTermOpenKey"){
        if(list[2] == "Server"){
            QStringList keyInfo = sendOpenKey(list[1]).split("&", Qt::SkipEmptyParts);
            Point longTermOpenKey;
            Point shortTermOpenKey;
            longTermOpenKey.x = (uint256_t)keyInfo[2].toStdString();
            longTermOpenKey.y = (uint256_t)keyInfo[3].toStdString();

            shortTermOpenKey.x = (uint256_t)list[3].toStdString();
            shortTermOpenKey.y = (uint256_t)list[4].toStdString();
            //qDebug() << "Before: " << QString::fromStdString(shortTermOpenKey.x.str()) << " " << QString::fromStdString(shortTermOpenKey.y.str());
            //qDebug() << "Before: " << QString::fromStdString(longTermOpenKey.x.str()) << " " << QString::fromStdString(longTermOpenKey.y.str());
            Point W = mqv(longTermOpenKey, shortTermOpenKey);
            qDebug() << "bef" << sockets[socket] << socket;
            QString x = "";
            QString y = "";
            for (int i = 0; i < 64; ++i) {
                x += QString::fromStdString(W.x.str())[i];
                y += QString::fromStdString(W.y.str())[i];
            }
            sockets[socket] = {x, y};
            qDebug() << "aft" << sockets[socket] << sockets.size();
            //qDebug() << "After: " << QString::fromStdString(shortTermOpenKey.x.str()) << " " << QString::fromStdString(shortTermOpenKey.y.str());
            //qDebug() << "Before: " << QString::fromStdString(longTermOpenKey.x.str()) << " " << QString::fromStdString(longTermOpenKey.y.str());
            return sendShortTermOpenKey(list[1]);
        }
    }
    //auth&userLogin&password&
    else if (list[0] == "auth") {
        return checkPassword(list[1], list[2]);
    }
    return "get it!";

}
