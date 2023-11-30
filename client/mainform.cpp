#include "mainform.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    this->setStyleSheet("QGroupBox::title { color : rgb(255, 218, 185); font-size: 150px;} ");

    ui->centralContest->setStyleSheet("QGroupBox::title{font-size:40pt;}");
    contestsWindow = new FormContests;
    infoContestWindow = new FormDetailInfo;
    authForm = new AuthForm;
    authForm->show();

    //ui->accountButton->setStyleSheet("background-color: rgb(194, 165, 140)");
    //ui->accountButton->setStyleSheet("color: rgb(0, 0, 0)");
    //ui->contestsButton->setStyleSheet("background-color: rgb(194, 165, 140)");
    //ui->contestsButton->setStyleSheet("color: rgb(194, 165, 140)");
    //ui->horsesButton->setStyleSheet("background-color: rgb(194, 165, 140)");
    //ui->horsesButton->setStyleSheet("color: rgb(194, 165, 140)");
    //ui->jockeysButton->setStyleSheet("background-color: rgb(194, 165, 140)");
    //ui->jockeysButton->setStyleSheet("color: rgb(194, 165, 140)");

    connect(authForm, &AuthForm::successLogin, this, &MainForm::anons);
    connect(Client::getInstance(), &Client::receivedFutureContestsForMain, this, &MainForm::renderInfo3Contests);

}

MainForm::~MainForm()
{
    delete ui;
}



void MainForm::anons()
{
    MainForm::show();
}


void MainForm::setNoInfoRight()
{
    ui->rightContest->setTitle(" ");
    ui->rightContestDate->setText(" ");
    ui->rightContestTime->setText("Нет информации");
    ui->rightContestTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->rightContestPlace->setText(" ");
    ui->rightContestRegStatus->setText(" ");
    ui->rightContestMoreInfo->setDisabled(true);
}


void MainForm::setNoInfoCentral()
{
    ui->centralContest->setTitle(" ");
    ui->centralContestDate->setText(" ");
    ui->centralContestTime->setText("Нет информации");
    ui->centralContestTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->centralContestPlace->setText(" ");
    ui->centralContestRegStatus->setText(" ");
    ui->centralContestMoreInfo->setDisabled(true);
}


void MainForm::setNoInfoLeft()
{
    ui->leftContest->setTitle(" ");
    ui->leftContestDate->setText(" ");
    ui->leftContestTime->setText("Нет информации");
    ui->leftContestTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->leftContestPlace->setText(" ");
    ui->leftContestRegStatus->setText(" ");
    ui->leftContestMoreInfo->setDisabled(true);
}


void MainForm::setInfoRight(Contest contest)
{
    ui->rightContest->setTitle(contest.getName());
    ui->rightContestDate->setText("Дата: " + contest.getRawDate());
    ui->rightContestTime->setText("Время: " + contest.getRawTime());
    ui->rightContestPlace->setText("Место: " + contest.getPlace());
    ui->rightContestRegStatus->setText("Регистрация: " + contest.getRegStatus());
    ui->rightContestMoreInfo->setProperty("id", contest.getId());
}


void MainForm::setInfoCentral(Contest contest)
{
    ui->centralContest->setTitle(contest.getName());
    ui->centralContestDate->setText("Дата: " + contest.getRawDate());
    ui->centralContestTime->setText("Время: " + contest.getRawTime());
    ui->centralContestPlace->setText("Место: " + contest.getPlace());
    ui->centralContestRegStatus->setText("Регистрация: " + contest.getRegStatus());
    ui->centralContestMoreInfo->setProperty("id", contest.getId());
}


void MainForm::setInfoLeft(Contest contest)
{
    ui->leftContest->setTitle(contest.getName());
    ui->leftContestDate->setText("Дата: " + contest.getRawDate());
    ui->leftContestTime->setText("Время: " + contest.getRawTime());
    ui->leftContestPlace->setText("Место: " + contest.getPlace());
    ui->leftContestRegStatus->setText("Регистрация: " + contest.getRegStatus());
    ui->leftContestMoreInfo->setProperty("id", contest.getId());
}


void MainForm::renderInfo3Contests(QVector<Contest> contests)
{
    if (contests.size() == 3) {
        setInfoRight(contests[0]);
        setInfoCentral(contests[1]);
        setInfoLeft(contests[2]);
    }
    else if (contests.size() == 2) {
        setNoInfoRight();
        setInfoCentral(contests[0]);
        setInfoLeft(contests[1]);
    }
    else if (contests.size() == 1) {
        setNoInfoRight();
        setInfoCentral(contests[0]);
        setNoInfoLeft();
    }
    else {
        setNoInfoRight();
        setNoInfoCentral();
        setNoInfoLeft();
    }
}


void MainForm::on_centralContestMoreInfo_clicked()
{
    qDebug() << ui->centralContestMoreInfo->property("id").toInt();
    requestDetailedInfoContest(QString::number(ui->centralContestMoreInfo->property("id").toInt()));
}


void MainForm::on_contestsButton_clicked()
{
    qDebug() << "contests";
    requestGeneralInfoContests();
}

