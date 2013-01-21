#ifndef LTFILESLOADER_H
#define LTFILESLOADER_H

#include <QList>
#include <QPair>
#include "../net/datastreamreader.h"


/*!
 * \brief The LTFilesLoader class is used to manage (load and save) .lt files. It can load both V1 and V2 .lt files version, but saves only in new format (V2) as the old one is no longer supported.
 */
class LTFilesLoader
{
public:
    LTFilesLoader();

    bool loadFile(QString fName, QVector< QPair<int, Packet> > &packets);
    bool loadV1File(QDataStream &stream, QVector< QPair<int, Packet> > &packets);
    bool loadV2File(QDataStream &stream, QVector< QPair<int, Packet> > &packets);

    void saveFile(QString fName, const QVector< QPair<int, Packet> > &packets);
    void saveMainData(QDataStream &stream);
    void savePackets(QDataStream &stream, const QVector< QPair<int, Packet> > &packets);
};

#endif // LTFILESLOADER_H
