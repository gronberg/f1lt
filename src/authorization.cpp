#include "authorization.h"

Authorization::Authorization(QObject *parent) :
    QObject(parent)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}


void Authorization::connectToServer(QString host, QString email, QString passwd)
{
    //QString host("http://live-timing.formula1.com/reg/login");
    QString content("application/x-www-form-urlencoded");

    QByteArray bArray(QString("email="+email+"&password="+passwd).toAscii());

    req = QNetworkRequest(host);
    req.setHeader(QNetworkRequest::ContentTypeHeader, content.toAscii());
    req.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(bArray.size()).toString());
    req.setRawHeader("User-Agent","live-f1 0.2.1");//  "Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.1.3) Gecko/20091020 Ubuntu/9.10 (karmic) Firefox/3.5.3 GTB7.0");

    manager.post(req, bArray);
}

void Authorization::finished(QNetworkReply *reply)
{
    QString v = qvariant_cast<QString>(reply->rawHeader("Set-Cookie"));
    parseCookie(v);

    emit cookieRecieved(cookie);
}

void Authorization::parseCookie(QString header)
{
    int idx = header.indexOf("USER=");
    if (idx == -1)
        return;

    idx += 5;

    int idx2 = header.indexOf(";");
    cookie = header.right(header.length() - idx).left(idx2-idx);
}
