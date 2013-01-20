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


#ifndef F1LTCORE_H
#define F1LTCORE_H

#define STR1(x) #x
#define STR(x) STR1(x)

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

/*!
 * \brief The F1LTCore class stores paths to application files. No object of this class can be created, all methods are static.
 */
class F1LTCore
{
public:    

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
        QDir dir = QDir::homePath() + "/.config/f1lt";

        if (!dir.exists())
        {
            if (!dir.mkpath(QDir::homePath() + "/.config/f1lt"))
                return QDir::homePath() + "/.f1lt.ini";
        }
        return QDir::homePath() + "/.config/f1lt/f1lt.ini";
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
        QDir dir = QDir::homePath() + "/.config/f1lt/ltdata";
        if (!dir.exists())
        {
            if (!dir.mkpath(QDir::homePath() + "/.config/f1lt/ltdata"))
            {
                return programHomeDir() + "/ltdata/";
            }
        }
        return QDir::homePath() + "/.config/f1lt/ltdata/";

#endif
    }

private:
    F1LTCore();
};

#endif // F1LTCORE_H
