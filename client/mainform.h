#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "client.h"
#include "authform.h"
#include "formdetailinfo.h"
#include "formcontests.h"
#include "formadminaccount.h"

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();
private slots:
    void anons();
    void renderInfo3Contests(QVector<Contest> contests);
    void on_centralContestMoreInfo_clicked();


    void on_contestsButton_clicked();

    void on_accountButton_clicked();

    void on_leftContestMoreInfo_clicked();

    void on_rightContestMoreInfo_clicked();

private:
    Ui::MainForm *ui;
    AuthForm *authForm;
    FormDetailInfo *infoContestWindow;
    FormContests *contestsWindow;

    FormAdminAccount *adminPanel;

    void setNoInfoRight();
    void setNoInfoCentral();
    void setNoInfoLeft();

    void setInfoRight(Contest contest);
    void setInfoCentral(Contest contest);
    void setInfoLeft(Contest contest);
};

#endif // MAINFORM_H
