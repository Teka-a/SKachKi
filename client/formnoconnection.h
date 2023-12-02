#ifndef FORMNOCONNECTION_H
#define FORMNOCONNECTION_H

#include <QWidget>

namespace Ui {
class FormNoConnection;
}

class FormNoConnection : public QWidget
{
    Q_OBJECT

public:
    explicit FormNoConnection(QWidget *parent = nullptr);
    ~FormNoConnection();

private:
    Ui::FormNoConnection *ui;
};

#endif // FORMNOCONNECTION_H
