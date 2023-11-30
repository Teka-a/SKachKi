#include "formcontests.h"
#include "ui_formcontests.h"

FormContests::FormContests(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormContests)
{
    ui->setupUi(this);
    connect(Client::getInstance(), &Client::receivedContests, this, &FormContests::setInfo);

}

FormContests::~FormContests()
{
    delete ui;
}


void FormContests::setInfo(QVector<Contest>)
{
    qDebug() << "here";

}
