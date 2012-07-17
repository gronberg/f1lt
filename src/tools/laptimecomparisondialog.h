#ifndef LAPTIMECOMPARISONDIALOG_H
#define LAPTIMECOMPARISONDIALOG_H

#include <QComboBox>
#include <QDialog>

#include <QList>
#include "../core/ltdata.h"
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

public slots:
    int exec();
    void show();
    void comboBoxValueChanged(int);
    void updateData();
    void updateCharts();
    void driverUpdated(const DriverData &dd)
    {
        for (int i = 0; i < 4; ++i)
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

private:
    Ui::LapTimeComparisonDialog *ui;

    QComboBox *comboBox[4];
    LapCompChart *lapCompChart;
    QColor color[4];
    QList<QPixmap> smallCarImg;

    bool reversedOrder;

    EventData &eventData;
};

#endif // LAPTIMECOMPARISONDIALOG_H
