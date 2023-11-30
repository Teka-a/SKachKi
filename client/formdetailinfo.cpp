#include "formdetailinfo.h"
#include "ui_formdetailinfo.h"

FormDetailInfo::FormDetailInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDetailInfo)
{
    regForm = new FormRegContest();
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    connect(Client::getInstance(), &Client::receivedContest, this, &FormDetailInfo::setDetailedInfo);

}

FormDetailInfo::~FormDetailInfo()
{
    delete ui;
}



void FormDetailInfo::setDetailedInfo(Contest contest, Place hippodrome, QVector<ParticipantPair> participants)
{
    this->contestId = contest.getId();
    ui->ContestName->setText(contest.getName());
    ui->contestDate->setText("Дата: " + contest.getFullDate());
    ui->contestTime->setText("Время: " + contest.getFullTime());
    ui->contestPlace->setText("Место: " + contest.getPlace());
    ui->contestDescription->setText("Описание: " + hippodrome.getDescription() + "\n" + hippodrome.getAddress());
    ui->contestPlacesJ->setText("Всего мест: 6");
    int registered = 0;
    for (int i = 0; i < 6; ++i) {
        if (participants[i].horseName == "-") {
            break;
        }
        else {
            registered += 1;
        }
    }
    ui->contestLJ->setText("Осталось мест: " + QString::number(6 - registered));
    ui->label_2->setStyleSheet("color : rgb(166, 166, 166);");
    ui->label_17->setStyleSheet("color : rgb(166, 166, 166);");
    ui->j1->setText(participants[0].jockey);
    ui->h1->setText(participants[0].horseName);

    ui->j2->setText(participants[1].jockey);
    ui->h2->setText(participants[1].horseName);

    ui->j3->setText(participants[2].jockey);
    ui->h3->setText(participants[2].horseName);

    ui->j4->setText(participants[3].jockey);
    ui->h4->setText(participants[3].horseName);

    ui->j5->setText(participants[4].jockey);
    ui->h5->setText(participants[4].horseName);

    ui->j6->setText(participants[5].jockey);
    ui->h6->setText(participants[5].horseName);

    qDebug() << contest.getFullDate() << contest.getFullTime();
    qDebug() << hippodrome.getName();
    qDebug() << participants[0].horseName;
    qDebug() << Client::getInstance()->getUserStatus();
    if (Client::getInstance()->getUserStatus() != "jockey") {
        ui->regButton->setVisible(false);
    }
    FormDetailInfo::show();
}



void FormDetailInfo::on_regButton_clicked()
{
    qDebug() << "reg";
    requestHorsesForContest(this->contestId);
    FormDetailInfo::close();
}

