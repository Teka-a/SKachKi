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

void requestPlaces()
{
    QString request = "requestPlaces&";
    Client::getInstance()->sendToServer(request);
}


void regForContest(QString contestId, QString horseId, QString jockeyLogin)
{
    QString request = "regForContest&" + contestId + "&" + horseId + "&" + jockeyLogin;
    Client::getInstance()->sendToServer(request);
}

void regNewUser(QString name, QString surname, QString phone, QString login, QString password, QString statusId, QString ltok, QString address, QString birthdate)
{
    QString request = "regNewUser&" + name + "&" + surname + "&" + statusId + "&"
                        + phone + "&" + address + "&" + birthdate + "&"
                        + login + "&" + password + "&" + ltok + "&";

    Client::getInstance()->sendToServer(request);
}


void regNewContest(QString name, QString date, QString time, QString placeId, QString status, QString info)
{
    QString request = "regNewContest&" + name + "&" + date + "&" + time + "&"
                      + placeId + "&" + status + "&" + info;
    Client::getInstance()->sendToServer(request);
}

void regNewPlace(QString name, QString address, QString description)
{
    QString request = "regNewPlace&" + name + "&" + address + "&" + description;
    Client::getInstance()->sendToServer(request);
}
