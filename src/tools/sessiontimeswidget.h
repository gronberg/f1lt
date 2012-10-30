#ifndef SESSIONTIMESWIDGET_H
#define SESSIONTIMESWIDGET_H


#include <QSettings>
#include <QTableWidgetItem>
#include <QWidget>

#include "../core/driverdata.h"
#include "../core/seasondata.h"

namespace Ui {
class SessionTimesWidget;
}

class SessionTimesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SessionTimesWidget(QWidget *parent = 0);
    ~SessionTimesWidget();
    
    void update();
    void handleRaceEvent();
    void handleQualiEvent();
    void handlePracticeEvent();

    void loadDriversList();

    int getNumber(int row);
    QString getName(int row);
    void exec();

    void removeRows(int row);

    void setFont(const QFont &);
    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void saveCheckedArray();
    void restoreCheckedArray();

private slots:
    void onHeaderClicked(int);
    void onHeaderDoubleClicked(int);

    void on_closeButton_clicked();

    void on_flagButton_clicked();

    void on_driversListWidget_clicked(const QModelIndex &index);

    void on_relativeButton_toggled(bool checked);

    void on_driversListWidget_doubleClicked(const QModelIndex &index);

private:

    QTableWidgetItem* setItem(int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                     QColor textColor = SeasonData::getInstance().getColor(LTPackets::DEFAULT), QBrush background = QBrush());

    Ui::SessionTimesWidget *ui;

    DriverData *selectedDriver;
    bool relativeTimes;
    QByteArray checkedArray;
};

#endif // SESSIONTIMESWIDGET_H
