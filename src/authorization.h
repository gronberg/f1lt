#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>

class Authorization : public QObject
{
    Q_OBJECT
public:
    explicit Authorization(QObject *parent = 0);

    void connectToServer(QString host, QString email, QString passwd);
    QString getCookie() { return cookie; }
    void parseCookie(QString header);

signals:
    void cookieRecieved(QString cookie);
    
public slots:
    void finished(QNetworkReply *);

private:
    QNetworkAccessManager manager;
    QNetworkRequest req;

    QString cookie;
    
};

#endif // AUTHORIZATION_H
