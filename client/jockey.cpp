#include "jockey.h"

Jockey::Jockey(QString id, QString name, QString surname, QString age, QString phone, QString rating)
{
    this->id = id.toInt();
    this->name = name;
    this->surname = surname;
    this->age = age;
    this->phone = phone;
    this->rating = rating;
}


int Jockey::generateRandomNum()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    bool ok;
    return partsUuid[1].toInt(&ok, 16);
}


QString Jockey::removeRandomNum()
{
    int originId = this->id - this->randomNum;
    return QString::number(originId);
}
