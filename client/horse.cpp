#include "horse.h"

Horse::Horse(QString id, QString name, QString sex, QString age, QString owner, QString rating)
{
    this->id = id.toInt();
    this->randomNum = generateRandomNum();


    this->name = name;
    this->sex = sex;
    this->age = age.toInt();
    this->owner = owner;
    this->rating = rating.toInt();
}


int Horse::generateRandomNum()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    bool ok;
    return partsUuid[1].toInt(&ok, 16);
}


QString Horse::removeRandomNum()
{
    int originId = this->id - this->randomNum;
    return QString::number(originId);
}


QString Horse::getId()
{
    return QString::number(this->id);
}


QString Horse::getName()
{
    return this->name;
}


QString Horse::getSex()
{
    return this->sex;
}


QString Horse::getAge()
{
    return QString::number(this->age);
}


QString Horse::getOwner()
{
    return this->owner;
}

QString Horse::getRating()
{
    return QString::number(this->rating);
}
