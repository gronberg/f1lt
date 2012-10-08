#ifndef LTFILESMANAGER_H
#define LTFILESMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>

#include <QObject>

class LTFilesManager : public QObject
{
    Q_OBJECT
public:
    explicit LTFilesManager(QObject *parent = 0);
    
signals:
    void ltListObtained(QStringList);
    void ltFileObtained(QByteArray);
    void downloadProgress ( qint64 bytesReceived, qint64 bytesTotal );
    void error ( QNetworkReply::NetworkError code );
    
public slots:
    void getLTList();
    void getLTFile(QString);

    void cancel();

    void finishedLTList();
    void finishedLTFile();    

private:
    QNetworkAccessManager manager;
    QNetworkRequest req;
    QNetworkReply *reply;
};

#endif // LTFILESMANAGER_H
