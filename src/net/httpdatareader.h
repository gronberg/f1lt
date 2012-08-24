#ifndef HttpDataReader_H
#define HttpDataReader_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpDataReader : public QObject
{
    Q_OBJECT
public:
    explicit HttpDataReader(QObject *parent = 0);
    ~HttpDataReader();

    void authorize(QString host, QString loginHost, QString email, QString passwd);    

    QString getCookie() { return cookie; }
    QString getHost() { return host; }
    void setHost(QString h) { host = h; }
    bool parseCookie(QString header);
    
signals:
    void cookieRecieved(QString cookie);
    void keyFrameObtained(const QByteArray &);
    void decryptionKeyObtained(unsigned int);
    void authorizationError();
    void error(QNetworkReply::NetworkError);
    
public slots:
    void finished();
    void obtainKeyFrame(unsigned int frame);
    void obtainDecryptionKey(unsigned int event_no);
    

private:
    QNetworkAccessManager manager;
    QNetworkRequest req;
    QNetworkReply *reply;

    QString cookie;
    QString host;
    QString loginHost;
    QByteArray keyFrame;
    unsigned int decryptionKey;

    long bytes;

    int state; //0 - doing nothing, 1 - authorizing, 2 - obtaining key frame, 3 - obtaining decryption key
};

#endif // HttpDataReader_H
