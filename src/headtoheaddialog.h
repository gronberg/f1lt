#ifndef HEADTOHEADDIALOG_H
#define HEADTOHEADDIALOG_H

#include <QDialog>
#include <QList>
#include "ltdata.h"
#include "chartwidget.h"

#include <QComboBox>

namespace Ui {
class HeadToHeadDialog;
}

class HeadToHeadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HeadToHeadDialog(bool rev = false, QWidget *parent = 0);
    ~HeadToHeadDialog();

    void setFont(const QFont &);

    void setReversedOrder(bool rev) { reversedOrder = rev; }
    bool isReversedOrder() { return reversedOrder; }

    QString calculateInterval(int driver1Idx, int driver2Idx, int lap);
    void loadCarImages();

public slots:
    int exec();
    void show();
    void comboBoxValueChanged(int);
    void updateData();
    void updateCharts();
    void driverUpdated(const DriverData &dd)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (dd.carID == eventData.getDriverId(comboBox[i]->currentText()))
            {
                updateData();
                updateCharts();
                return;
            }
        }
    }
    
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_pushButton_clicked();

private:
    Ui::HeadToHeadDialog *ui;

    QComboBox *comboBox[2];
    LapCompChartWidget *lapCompChart;
    GapCompChartWidget *gapCompChart;
    PosCompChartWidget *posCompChart;
    QColor color[2];
    QList<QPixmap> smallCarImg;

    bool reversedOrder;

    EventData &eventData;
};

#endif // HEADTOHEADDIALOG_H
