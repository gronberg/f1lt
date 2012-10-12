#ifndef LTWIDGET_H
#define LTWIDGET_H

#include <QItemDelegate>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QWidget>

#include "../core/eventdata.h"
#include "ltitemdelegate.h"
#include "models/ltmodel.h"




namespace Ui {
class LTWidget;
}

class LTWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LTWidget(QWidget *parent = 0);
    ~LTWidget();

    void updateLT();

    virtual void setFont(const QFont &font);


//    void updateRaceEvent(int ddIdx = -1);
//    void updateDriver(int driverIdx);
//    void updatePracticeEvent(int ddIdx = -1);
//    void updateQualiEvent(int ddIdx = -1);

    void loadCarImages();

    void setDrawCarThumbnails(bool val)
    {
        drawCarThumbnails = val;
        itemDelegate->setDrawCarThumbnails(val);
        resizeEvent(0);
        updateLT();
    }

    bool printDiff(int row, int col) { }

    void clearData();

    void clearModelsData()
    {
        if (ltModel != 0)
            ltModel->clearData();
    }
    int getCurrentDriverId() { return currDriverId; }


protected:
    virtual void resizeEvent(QResizeEvent *);
    
signals:
    void driverSelected(int id);
    void driverDoubleClicked(int id);

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_headerClicked(int column);

private:
    Ui::LTWidget *ui;

    QList<QPixmap> carImg;

    EventData &eventData;
    LTPackets::EventType eventType;

    int currDriverId;

    bool drawCarThumbnails;
    int showDiff;	//1 - time (best, q1), 2 - q2, 3 - q3, 4 - interval

    LTModel *ltModel;
    LTMainItemDelegate *itemDelegate;    
};

#endif // LTWIDGET_H
