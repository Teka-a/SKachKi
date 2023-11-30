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
    QString request = "auth&" + Client::getInstance()->getLogin() + "&" + Client::getInstance()->getPassword();
    Client::getInstance()->sendToServer(request);
}

void requestGeneralInfoContests()
{
    QString request = "requestContest&generalInfo";
    Client::getInstance()->sendToServer(request);
}

void requestDetailedInfoContest(QString id)
{
    QString request = "requestContest&" + id;
    Client::getInstance()->sendToServer(request);
}

void request3Contests()
{
    QString request = "requestContest&mainPage";
    Client::getInstance()->sendToServer(request);
}

void requestHorsesForContest(QString id)
{
    QString request = "requestHorsesForContest&" + id + "&";
    Client::getInstance()->sendToServer(request);
}

void regForContest(QString contestId, QString horseId, QString jockeyLogin)
{
    QString request = "regForContest&" + contestId + "&" + horseId + "&" + jockeyLogin;
    Client::getInstance()->sendToServer(request);
}
