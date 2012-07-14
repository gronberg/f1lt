/********************************************************************************
** Form generated from reading UI file 'eventplayer.ui'
**
** Created: Sun Jul 15 00:11:39 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTPLAYER_H
#define UI_EVENTPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventPlayer
{
public:
    QHBoxLayout *horizontalLayout;
    QComboBox *speedBox;
    QPushButton *rewindToStartButton;
    QPushButton *rewindButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *forwardButton;
    QPushButton *forwardToEndButton;
    QSlider *seekSlider;
    QLabel *timeLabel;

    void setupUi(QWidget *EventPlayer)
    {
        if (EventPlayer->objectName().isEmpty())
            EventPlayer->setObjectName(QString::fromUtf8("EventPlayer"));
        EventPlayer->resize(551, 40);
        EventPlayer->setMaximumSize(QSize(551, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        EventPlayer->setFont(font);
        horizontalLayout = new QHBoxLayout(EventPlayer);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(4, 4, 4, 4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        speedBox = new QComboBox(EventPlayer);
        speedBox->setObjectName(QString::fromUtf8("speedBox"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        speedBox->setFont(font1);
        speedBox->setEditable(false);
        speedBox->setFrame(true);

        horizontalLayout->addWidget(speedBox);

        rewindToStartButton = new QPushButton(EventPlayer);
        rewindToStartButton->setObjectName(QString::fromUtf8("rewindToStartButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/skip-backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        rewindToStartButton->setIcon(icon);
        rewindToStartButton->setFlat(true);

        horizontalLayout->addWidget(rewindToStartButton);

        rewindButton = new QPushButton(EventPlayer);
        rewindButton->setObjectName(QString::fromUtf8("rewindButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ui_icons/rewind.png"), QSize(), QIcon::Normal, QIcon::Off);
        rewindButton->setIcon(icon1);
        rewindButton->setFlat(true);

        horizontalLayout->addWidget(rewindButton);

        playButton = new QPushButton(EventPlayer);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ui_icons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon2);
        playButton->setFlat(true);

        horizontalLayout->addWidget(playButton);

        stopButton = new QPushButton(EventPlayer);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/ui_icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon3);
        stopButton->setFlat(true);

        horizontalLayout->addWidget(stopButton);

        forwardButton = new QPushButton(EventPlayer);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ui_icons/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        forwardButton->setIcon(icon4);
        forwardButton->setFlat(true);

        horizontalLayout->addWidget(forwardButton);

        forwardToEndButton = new QPushButton(EventPlayer);
        forwardToEndButton->setObjectName(QString::fromUtf8("forwardToEndButton"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/ui_icons/skip-forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        forwardToEndButton->setIcon(icon5);
        forwardToEndButton->setFlat(true);

        horizontalLayout->addWidget(forwardToEndButton);

        seekSlider = new QSlider(EventPlayer);
        seekSlider->setObjectName(QString::fromUtf8("seekSlider"));
        seekSlider->setMinimumSize(QSize(100, 0));
        seekSlider->setMaximumSize(QSize(100, 16777215));
        seekSlider->setSingleStep(1);
        seekSlider->setPageStep(100);
        seekSlider->setSliderPosition(0);
        seekSlider->setTracking(true);
        seekSlider->setOrientation(Qt::Horizontal);
        seekSlider->setInvertedAppearance(false);
        seekSlider->setTickPosition(QSlider::NoTicks);
        seekSlider->setTickInterval(100);

        horizontalLayout->addWidget(seekSlider);

        timeLabel = new QLabel(EventPlayer);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        timeLabel->setFont(font2);
        timeLabel->setFrameShape(QFrame::NoFrame);
        timeLabel->setLineWidth(1);
        timeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        timeLabel->setMargin(0);
        timeLabel->setIndent(13);

        horizontalLayout->addWidget(timeLabel);


        retranslateUi(EventPlayer);

        speedBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(EventPlayer);
    } // setupUi

    void retranslateUi(QWidget *EventPlayer)
    {
        EventPlayer->setWindowTitle(QApplication::translate("EventPlayer", "Form", 0, QApplication::UnicodeUTF8));
        speedBox->clear();
        speedBox->insertItems(0, QStringList()
         << QApplication::translate("EventPlayer", "x 0.5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EventPlayer", "x 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EventPlayer", "x 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EventPlayer", "x 4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EventPlayer", "x 8", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        speedBox->setToolTip(QApplication::translate("EventPlayer", "Playing speed", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        rewindToStartButton->setToolTip(QApplication::translate("EventPlayer", "Skip to begin", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rewindToStartButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        rewindButton->setToolTip(QApplication::translate("EventPlayer", "Rewind", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rewindButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        playButton->setToolTip(QApplication::translate("EventPlayer", "Play/Pause", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        playButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("EventPlayer", "Stop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        forwardButton->setToolTip(QApplication::translate("EventPlayer", "Fast forward", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        forwardButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        forwardToEndButton->setToolTip(QApplication::translate("EventPlayer", "Skip to end", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        forwardToEndButton->setText(QString());
        timeLabel->setText(QApplication::translate("EventPlayer", "0:00:00/0:00:00", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventPlayer: public Ui_EventPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTPLAYER_H
