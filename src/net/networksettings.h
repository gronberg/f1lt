/*******************************************************************************
 *                                                                             *
 *   F1LT - unofficial Formula 1 live timing application                       *
 *   Copyright (C) 2012-2013  Mariusz Pilarek (pieczaro@gmail.com)             *
 *                                                                             *
 *   This program is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation, either version 3 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                             *
 *******************************************************************************/



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

    QString getVersionUrl()
    {
        return versionUrl;
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

    QString versionUrl;

    bool proxyOn;
    QNetworkProxy proxy;
};

#endif // NETWORKSETTINGS_H
