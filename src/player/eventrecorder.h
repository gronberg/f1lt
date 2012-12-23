#ifndef EVENTRECORDER_H
#define EVENTRECORDER_H

#include <QObject>
#include "../net/datastreamreader.h"
#include "../core/sessiontimer.h"

class EventRecorder : public QObject
{
    Q_OBJECT
public:
    explicit EventRecorder(SessionTimer *st, QObject *parent = 0);
    void saveToFile(QString fName);
    void saveLTInfo(QDataStream &);
    void savePackets(QDataStream &);

    void gatherInitialData();
    void gatherSysData();
    void gatherDriverData();

    bool isEmpty() { return packets.isEmpty(); }
    bool isSessionRecorded() { return sessionRecorded; }
    void setSessionRecorded(bool rec) { sessionRecorded = rec; }

    void setAutoStopRecord(int x)
    {
    	autoStopRecord = x;
    }

    void appendSessionTimer();
//    void saveEventData(QDataStream &);
//    void saveLapData(QDataStream &);

//    void storeLapData(const DriverData &);
    
signals:
	void recordingStopped();
    
public slots:
    void startRecording();
    void stopRecording();
    void timeout();
    void appendPacket(const Packet&);
    void appendPacket(const QPair<Packet, qint64> &packet);
//    void updateEventData(const EventData &);
//    void updateDriverData(const DriverData &);

private:
//    EventData eventData;
//    QList<EventData> eventDataList;

//    //drivers lap data will be stored here, in a 2-dimensional array
//    QList< QList<LapData> > lapDataList;
    QList< QPair<int, Packet> > packets;
    QVector<LTTeam> ltTeamList;
    LTEvent ltEvent;

    EventData &eventData;
    
    qint64 recordStartTime;
    int elapsedSeconds;
    int autoStopRecord;
    int elapsedTimeToStop;
    bool sessionRecorded;

    QPair<int, QTime> lastSavedTime;

    SessionTimer *sessionTimer;
};

#endif // EVENTRECORDER_H
