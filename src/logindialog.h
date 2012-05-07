#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QString getEmail();
    QString getPasswd();
    int exec(QString email="", QString passwd="");
    
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
