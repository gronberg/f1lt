#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QNetworkProxy>
#include <QSettings>
#include <QString>

class NetworkSettings
{
public:

    static NetworkSettings &getInstance()
    {
        static NetworkSettings instance;
        return instance;
    }

    QString getUser()
    {
        return user;
    }
    void setUserPassword(QString u, QString p)
    {
        user = u;
        passwd = encodePasswd(p);
    }

    QString getPassword()
    {
        return encodePasswd(passwd);
    }

    QString getHttpHost()
    {
        return httpHost;
    }

    QString getHttpLoginUrl()
    {
        return httpLoginUrl;
    }

    QString getHttpKeyFrameUrl()
    {
        return httpKeyFrameUrl;
    }

    QString getHttpDecryptionKeyUrl()
    {
        return httpDecryptionKeyUrl;
    }

    QString getSocketHost()
    {
        return socketHost;
    }

    int getSocketPort()
    {
        return socketPort;
    }

    QString getLTDataUrl()
    {
        return ltDataUrl;
    }

    QString getLTDataList()
    {
        return ltDataList;
    }

    bool usingProxy()
    {
        return proxyOn;
    }

    QNetworkProxy getProxy()
    {
        return proxy;
    }

    void setProxy(const QNetworkProxy &p, bool pOn = false)
    {
        proxy = p;
        proxyOn = pOn;
    }

    void loadSettings(const QSettings &settings);
    void saveSettings(QSettings &settings);

private:

    NetworkSettings();

    //very simple passwd encoding algorithm
    QString encodePasswd(QString passwd)
    {
        int sz = passwd.size();
        QString ret;
        for (int i = 0; i < sz; ++i)
        {
            char c = passwd[i].toAscii();
            c ^= (1 << (i%8));
            ret += c;
        }
        return ret;
    }

    QString user;
    QString passwd;

    QString httpHost;
    QString httpLoginUrl;
    QString httpKeyFrameUrl;
    QString httpDecryptionKeyUrl;

    QString socketHost;
    int socketPort;

    QString ltDataUrl;
    QString ltDataList;

    bool proxyOn;
    QNetworkProxy proxy;
};

#endif // NETWORKSETTINGS_H
