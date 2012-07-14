#ifndef F1LTCORE_H
#define F1LTCORE_H

#define STR1(x) #x
#define STR(x) STR1(x)

#include <QCoreApplication>
#include <QDir>

class F1LTCore
{
public:
    F1LTCore();

    static QString programVersion() { return "1.2.0"; }


    static QString programHomeDir()
    {
#ifdef WIN32
        return QCoreApplication::applicationDirPath();
#else
        return QCoreApplication::applicationDirPath();//QDir::homePath() + "/.f1lt";
#endif
    }

    static QString iniFile()
    {
        return programHomeDir() + "/f1lt.ini";
    }

    static QString seasonDataFile()
    {
        //QString prefix = STR(INSTALL_PREFIX);
        //return prefix+"share/F1LT/season.dat";
        return programHomeDir() + "/season.dat";
    }

    static QString ltDataHomeDir()
    {
        return programHomeDir() + "/ltdata/";
    }
};

#endif // F1LTCORE_H
