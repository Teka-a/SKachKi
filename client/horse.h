#ifndef HORSE_H
#define HORSE_H

#include <QString>
#include <QRegularExpression>
#include <QUuid>
#include <QDebug>


class Horse
{
private:
    int randomNum;
    int id;
    QString name;
    QString sex;
    int age;
    QString owner;
    int rating;
    bool isAvailable;
    int generateRandomNum();
public:
    Horse(QString id, QString name, QString sex, QString age, QString owner, QString rating = "0");

    QString getId();
    QString getName();
    QString getSex();
    QString getAge();
    QString getOwner();
    QString getRating();
    QString getAvailability();
    QString removeRandomNum();
};

#endif // HORSE_H
