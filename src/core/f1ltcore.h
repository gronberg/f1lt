#ifndef F1LTCORE_H
#define F1LTCORE_H

#define STR1(x) #x
#define STR(x) STR1(x)

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

class F1LTCore
{
public:
    F1LTCore();

    static QString programVersion() { return "2.0.0"; }


    static QString programHomeDir()
    {
#ifdef WIN32
        return QCoreApplication::applicationDirPath();
#else
        return QCoreApplication::applicationDirPath();
#endif
    }

    static QString iniFile()
    {
#ifdef WIN32
        return programHomeDir() + "/f1lt.ini";
#else
        QDir dir = QDir::homePath() + "/.f1lt";

        if (!dir.exists())
        {
            if (!dir.mkpath(QDir::homePath() + "/.f1lt"))
                return QDir::homePath() + "/.f1lt.ini";
        }
        return QDir::homePath() + "/.f1lt/f1lt.ini";
#endif
    }

    static QString seasonDataFile()
    {
        //QString prefix = STR(INSTALL_PREFIX);
        //return prefix+"share/F1LT/season.dat";
#ifdef WIN32
        return programHomeDir() + "/season.dat";
#else
        QString prefix = STR(INSTALL_PREFIX);
        QDir dir;
        if (dir.exists(prefix))
            return prefix + "/share/season.dat";
        else
            return programHomeDir() + "/season.dat";
#endif
    }

    static QString trackDataFile()
    {
#ifdef WIN32
        return programHomeDir() + "/trackdata.dat";
#else
        QString prefix = STR(INSTALL_PREFIX);
        QDir dir;
        if (dir.exists(prefix))
            return prefix + "/share/trackdata.dat";
        else
            return programHomeDir() + "/trackdata.dat";
#endif
    }

    static QString trackRercordsFile()
    {
#ifdef WIN32
        return programHomeDir() + "/trackrecords.dat";
#else
        QString prefix = STR(INSTALL_PREFIX);
        QDir dir;
        if (dir.exists(prefix))
            return prefix + "/share/trackrecords.dat";
        else
            return programHomeDir() + "/trackrecords.dat";
#endif
    }

    static QString ltDataHomeDir()
    {
#ifdef WIN32
        QString path = programHomeDir() + "/ltdata/";
        QDir dir(path);
        if (!dir.exists())
            dir.mkpath(path);

        return path;
#else
        QDir dir = QDir::homePath() + "/.f1lt/ltdata";
        if (!dir.exists())
        {
            if (!dir.mkpath(QDir::homePath() + "/.f1lt/ltdata"))
            {
                return programHomeDir() + "/ltdata/";
            }
        }
        return QDir::homePath() + "/.f1lt/ltdata/";

#endif
    }

    static QString ltDataUrl()
    {
        return "http://lt.f1lt.pl/";
    }

    static QString ltDataList()
    {
        return "http://lt.f1lt.pl/lis.php";
    }
};

#endif // F1LTCORE_H
