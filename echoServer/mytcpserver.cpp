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
        Functions f;
        qDebug() << f.checkPassword("ohno", "password");

        qDebug() << "SKachKi server is started";
        Curve *curveSecp256k1 = Curve::getInstance();
        Database *db = Database::getInstance();
        Point i = curveSecp256k1->countComposition(83666);
        qDebug() << "Here mullt: " << QString::fromStdString(i.x.str()) << " " << QString::fromStdString(i.y.str());
        QString m = "password";
        m = m.toUtf8().toHex();
        qDebug() << "Hash" << getHash512(m);
        m = "&auth&userLogin&password&";
        QStringList list = m.split("&", Qt::SkipEmptyParts);
        qDebug() << "List" << list;
        std::string plaintext = "user1&getKey&eofprfoerofrpfop";
        std::string key = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
        std::string IV =  "1234567890abcef0a1b2c3d4e5f0011223344556677889901213141516171819";
        qDebug() << QString::fromStdString(plaintext);
        //std::cout << "Enter key: ";
        //getline(std::cin, key);
        //std::cout << "Enter IV: ";
        //getline(std::cin, IV);
        m = encryptKuznechikOFB(QString::fromStdString(plaintext).toUtf8().toHex(), QString::fromStdString(key), QString::fromStdString(IV));
        QString p = decryptKuznechikOFB(m, QString::fromStdString(key), QString::fromStdString(IV));
        QByteArray hex = QByteArray::fromHex(p.toUtf8());
        qDebug() << hex.data();
        QString str = QString::fromUtf8(hex);
    }
}


/*создание слота для нового подключения*/
void MyTcpServer::slotNewConnection()
{
    QTcpSocket* mTcpSocket = mTcpServer->nextPendingConnection();
    list.push_back(mTcpSocket);
    QVector<QString> params {"4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048", "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea"};
    sockets[mTcpSocket] = params;
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
    //qDebug() << mTcpSocket;
    QVector<QString> params = sockets[mTcpSocket];
    QString res = "";
    while (mTcpSocket->bytesAvailable()>0) {
        QByteArray array = mTcpSocket->readAll();
        res.append(array);
    }
    QString array = decryptKuznechikOFB(res, params[0], params[1]);
    QByteArray hex = QByteArray::fromHex(array.toUtf8());
    array = hex.data();

    res = f.parse(array, sockets, mTcpSocket);
    QStringList list = res.split("&", Qt::SkipEmptyParts);
    res = encryptKuznechikOFB(res.toUtf8().toHex(), params[0], params[1]);
    if (list[1] == "failed") {
        qDebug() << "Params have been set to default";
        params = {"4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048", "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea"};
        sockets[mTcpSocket] = params;
    }
    //qDebug() << "Encrypted" << res;
    mTcpSocket->write(res.toUtf8());

}

/*возможность подключения нескольких клиентов*/
void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* mTcpSocket = (QTcpSocket*)sender();
    //qDebug() << "dicsonnected";
    list.removeAt(list.indexOf(mTcpSocket));
    sockets.remove(mTcpSocket);
    mTcpSocket->close();
    qDebug() << sockets.size();
}
