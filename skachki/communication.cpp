#include "communication.h"

void requestOpenKey(QString userLogin)
{
    QString request = "requestOpenKey&" + userLogin;
    Client::getInstance()->sendToServer(request);
}


void sendShortTermOpenKey(Point shortTermOpenKey, QString to)
{
    QString request = "shortTermOpenKey&" + Client::getInstance()->getLogin() + "&" + to + "&" + QString::fromStdString(shortTermOpenKey.x.str()) + "&" + QString::fromStdString(shortTermOpenKey.y.str());
    Client::getInstance()->sendToServer(request);
}

void sendCredentials()
{
    QString request = "&auth&" + Client::getInstance()->getLogin() + "&" + Client::getInstance()->getPassword();
    Client::getInstance()->sendToServer(request);
}
