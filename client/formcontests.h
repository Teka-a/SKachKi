#ifndef FORMCONTESTS_H
#define FORMCONTESTS_H

#include <QWidget>
#include "client.h"
#include "communication.h"


namespace Ui {
class FormContests;
}

class FormContests : public QWidget
{
    Q_OBJECT

public:
    explicit FormContests(QWidget *parent = nullptr);
    ~FormContests();
private slots:
    void setInfo(QVector<Contest>);
private:
    Ui::FormContests *ui;
};

#endif // FORMCONTESTS_H
