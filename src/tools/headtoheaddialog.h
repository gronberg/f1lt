#ifndef HEADTOHEADDIALOG_H
#define HEADTOHEADDIALOG_H

#include <QDialog>
#include <QList>
#include "../core/seasondata.h"
#include "../charts/lapcompchart.h"

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

    void loadCarImages();

    int getNumber(int);
    void setCurrentDriver(int id);  

public slots:
    int exec(int currentDriverId = 0);
    void show(int currentDriverId = 0);
    void comboBoxValueChanged(int);
    void updateData();
    void updateCharts();
    void driverUpdated(const DriverData &dd)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (dd.getCarID() == eventData.getDriverId(getNumber(i)))
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
    LapCompChart *lapCompChart;
    GapCompChart *gapCompChart;
    PosCompChart *posCompChart;
    QColor color[2];
    QList<QPixmap> *smallCarImg;

    bool reversedOrder;

    EventData &eventData;
    int thumbnailsSize;
};

#endif // HEADTOHEADDIALOG_H
