#include "httpdatareader.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

HttpDataReader::HttpDataReader(QObject *parent) :
    QObject(parent), state(0)
{
    bytes = 0;
}

HttpDataReader::~HttpDataReader()
{
    qDebug()<<"HTTP bytes received = "<<bytes;
}

void HttpDataReader::authorize(QString host, QString loginHost, QString email, QString passwd)
{
    this->host = host;
    this->loginHost = loginHost;
    QString authHost = loginHost + "/reg/login";
    QString content("application/x-www-form-urlencoded");

    QByteArray bArray(QString("email="+email+"&password="+passwd).toAscii());

    req = QNetworkRequest(authHost);
    req.setHeader(QNetworkRequest::ContentTypeHeader, content.toAscii());
    req.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(bArray.size()).toString());
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.post(req, bArray);

    state = 1;

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpDataReader::obtainKeyFrame(unsigned int frame)
{
    if (state != 0)
        return;


    QString keyhost = host + "/keyframe";

//    QFile f("/tmp/keyframes");
//    f.open(QIODevice::Append);
//    QTextStream ts(&f);

//    std::cout<<"frame="<<frame<<std::endl;
    if (frame > 0)
    {
        QString k = QString("%1").arg(frame);

//        std::cout<<k.length()<<std::endl;
        int size = k.size();
        for (int i = 0; i < 5 - size; ++i)
        {
            k = "0" + k;
//            std::cout<<"k="<<k.toStdString()<<std::endl;
        }

        keyhost += "_" + k;
        //keyhost += "_" + QString("%1").arg(frame, 5, 10, cc);
    }

    keyhost += ".bin";

//    ts << keyhost << "\n";
//    ts.flush();
//    f.close();

    req = QNetworkRequest(keyhost);
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.get(req);

    state = 2;

    qDebug(keyhost.toStdString().c_str());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpDataReader::obtainDecryptionKey(unsigned int event_no)
{
    QString keyhost = host + "/reg/getkey/" + QString("%1").arg(event_no) + ".asp?auth=" + cookie;


    req = QNetworkRequest(keyhost);
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.get(req);

    state = 3;

    qDebug(keyhost.toStdString().c_str());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

//void HttpDataReader::obtainTotalLaps()
//{
//    QString host = "http://live-f1.puseyuk.co.uk/laps.php";


//    req = QNetworkRequest(host);
//    req.setRawHeader("User-Agent","live-f1 0.2.1");//  "Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.1.3) Gecko/20091020 Ubuntu/9.10 (karmic) Firefox/3.5.3 GTB7.0");

//    reply = manager.get(req);

//    state = 4;

//    qDebug(host.toStdString().c_str());
//    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
//    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
//    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
//}

void HttpDataReader::finished()
{
    if (state == 1)
    {
        QString v = qvariant_cast<QString>(reply->rawHeader("Set-Cookie"));

        state = 0;
        if (parseCookie(v))
            emit cookieRecieved(cookie);        
    }
    else if (state == 2)
    {
        keyFrame = reply->readAll();
        bytes += keyFrame.size();

        state = 0;

        emit keyFrameObtained(keyFrame);
    }
    else if (state == 3)
    {
        QByteArray buf = reply->readAll();
        decryptionKey = 0;

        bytes += buf.size();

        for (int i = 0; i < buf.size(); ++i)
        {
            if ((buf[i] >= '0') && (buf[i] <= '9'))
                decryptionKey = (decryptionKey << 4) | (buf[i] - '0');

            else if ((buf[i] >= 'a') && (buf[i] <= 'f'))
                decryptionKey = (decryptionKey << 4) | (buf[i] - 'a' + 10);

            else if ((buf[i] >= 'A') && (buf[i] <= 'F'))
                decryptionKey = (decryptionKey << 4) | (buf[i] - 'A' + 10);

            else break;
        }
        state = 0;

        emit decryptionKeyObtained(decryptionKey);
    }
}

bool HttpDataReader::parseCookie(QString header)
{
    int idx = header.indexOf("USER=");
    if (idx == -1)
    {
        emit authorizationError();
        return false;
    }

    idx += 5;

    int idx2 = header.indexOf(";");
    cookie = header.right(header.length() - idx).left(idx2-idx);

    return true;
}
