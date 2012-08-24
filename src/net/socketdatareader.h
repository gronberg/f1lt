#ifndef SocketDataReader_H
#define SocketDataReader_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTimer>

class SocketDataReader : public QObject
{
    Q_OBJECT

public:
    void runS();
    SocketDataReader(QObject *parent=0);
    ~SocketDataReader();
    void openStream(QString host, int port);

    void connectToHost();
    void disconnectFromHost();

    void wakeUpServer();

signals:
    void streamOpened();
    void packetObtained(const QByteArray &);
    void error(QAbstractSocket::SocketError);

public slots:
    void connected();    
    void readyRead();
    void timeout();
    void connectionError(QAbstractSocket::SocketError);
    void reconnect();

private:
    QTcpSocket *socket;
    QByteArray data;
    QString host;
    int port;

    QTimer *timer;
    QTimer *reconnectTimer;

    bool connectionOpened;

    int tryReconnect;

    long bytes;
};

#endif // SocketDataReader_H
