#ifndef EVENTRECORDER_H
#define EVENTRECORDER_H

#include <QObject>
#include "../core/datastreamreader.h"

class EventRecorder : public QObject
{
    Q_OBJECT
public:
    explicit EventRecorder(QObject *parent = 0);
    void saveToFile(QString fName);
    void saveLTInfo(QDataStream &);
    void savePackets(QDataStream &);

    void gatherInitialData();
    void gatherSysData();
    void gatherDriverData();

    bool isEmpty() { return packets.isEmpty(); }

    void setAutoStopRecord(int x)
    {
    	autoStopRecord = x;
    }
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
//    void updateEventData(const EventData &);
//    void updateDriverData(const DriverData &);

private:
//    EventData eventData;
//    QList<EventData> eventDataList;

//    //drivers lap data will be stored here, in a 2-dimensional array
//    QList< QList<LapData> > lapDataList;
    QList< QPair<int, Packet> > packets;
    QList<LTTeam> ltTeamList;
    LTEvent ltEvent;

    EventData &eventData;
    
    int elapsedSeconds;
    int autoStopRecord;
    int elapsedTimeToStop;
};

#endif // EVENTRECORDER_H
