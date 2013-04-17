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


#include "networksettings.h"

NetworkSettings::NetworkSettings()
{
    httpHost = "http://live-timing.formula1.com";
    httpLoginUrl = "http://formula1.com/reg/login/";
    httpKeyFrameUrl = "http://live-timing.formula1.com/keyframe";
    httpDecryptionKeyUrl = "http://live-timing.formula1.com/reg/getkey/";

    socketHost = "live-timing.formula1.com";
    socketPort = 4321;

//    socketHost = "192.168.1.2";
//    socketPort = 6666;

    ltDataUrl = "http://lt.f1lt.pl/";
    ltDataList = "http://lt.f1lt.pl/lis.php";

    versionUrl = "http://f1lt.pl/version";
}

void NetworkSettings::loadSettings(const QSettings &settings)
{
    user = settings.value("login/email").toString();
    passwd = settings.value("login/passwd").toString();

    QString proxyHost = settings.value("proxy/host").toString();
    int proxyPort = settings.value("proxy/port", 0).toInt();
    int proxyType = settings.value("proxy/type", 0).toInt();
    QString proxyUser = settings.value("proxy/user").toString();
    QString proxyPasswd = encodePasswd(settings.value("proxy/passwd").toString());
    proxyOn = settings.value("proxy/enabled", false).toBool();

    proxy.setHostName(proxyHost);
    proxy.setPort(proxyPort);
    proxy.setType(QNetworkProxy::ProxyType(proxyType));
    proxy.setUser(proxyUser);
    proxy.setPassword(proxyPasswd);
}

void NetworkSettings::saveSettings(QSettings &settings)
{
    settings.setValue("login/email", user);
    settings.setValue("login/passwd", passwd);
    settings.setValue("proxy/host", proxy.hostName());
    settings.setValue("proxy/port", proxy.port());
    settings.setValue("proxy/type", (int)proxy.type());
    settings.setValue("proxy/user", proxy.user());
    settings.setValue("proxy/passwd", encodePasswd(proxy.password()));
    settings.setValue("proxy/enabled", proxyOn);
}
