#include "networksettings.h"

NetworkSettings::NetworkSettings()
{
    httpHost = "http://live-timing.formula1.com";
    httpLoginUrl = "http://formula1.com/reg/login/";
    httpKeyFrameUrl = "http://live-timing.formula1.com/keyframe";
    httpDecryptionKeyUrl = "http://live-timing.formula1.com/reg/getkey/";

    socketHost = "live-timing.formula1.com";
    socketPort = 4321;

    ltDataUrl = "http://lt.f1lt.pl/";
    ltDataList = "http://lt.f1lt.pl/lis.php";
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
}
