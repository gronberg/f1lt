#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkProxy>

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
    QNetworkProxy getProxy();
    int exec(QString email="", QString passwd="", QNetworkProxy proxy = QNetworkProxy());
    
private slots:

    void on_proxyCheckBox_toggled(bool checked);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
