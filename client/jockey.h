#ifndef JOCKEY_H
#define JOCKEY_H


#include <QString>
#include <QDate>
#include <QTime>
#include <QRegularExpression>
#include <QUuid>
#include <QDebug>


class Jockey
{
private:
    int randomNum;
    int id;
    QString name;
    QString surname;
    QString age;
    QString phone;
    QString rating;

    int generateRandomNum();
public:
    Jockey(QString id, QString name, QString surname, QString age, QString phone, QString rating);

    QString getId();
    QString getName();
    QString getAge();
    QString getPhone();
    QString getRating();
    QString getSurname();
    QString removeRandomNum();
};

#endif // JOCKEY_H
