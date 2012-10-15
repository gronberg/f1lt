#ifndef LAPTIMECOMPARISONDIALOG_H
#define LAPTIMECOMPARISONDIALOG_H

#include <QComboBox>
#include <QDialog>

#include <QList>
#include "../core/seasondata.h"
#include "../charts/lapcompchart.h"

namespace Ui {
class LapTimeComparisonDialog;
}

class LapTimeComparisonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LapTimeComparisonDialog(bool rev = false, QWidget *parent = 0);
    ~LapTimeComparisonDialog();
    
    void setFont(const QFont &);

    void setReversedOrder(bool rev) { reversedOrder = rev; }
    bool isReversedOrder() { return reversedOrder; }

    void loadCarImages();

    int getNumber(int);
    void setCurrentDriver(int id);

public slots:
    int exec(int currentCarId = 0);
    void show(int currentCarId = 0);
    void comboBoxValueChanged(int);
    void updateData();
    void updateCharts();
    void driverUpdated(const DriverData &dd)
    {
        for (int i = 0; i < 4; ++i)
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

private:
    Ui::LapTimeComparisonDialog *ui;

    QComboBox *comboBox[4];
    LapCompChart *lapCompChart;
    QColor color[4];
    QList<QPixmap> *smallCarImg;

    bool reversedOrder;

    EventData &eventData;
    int thumbnailsSize;
};

#endif // LAPTIMECOMPARISONDIALOG_H
