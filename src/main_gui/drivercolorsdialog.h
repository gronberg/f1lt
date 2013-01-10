#ifndef DRIVERCOLORSDIALOG_H
#define DRIVERCOLORSDIALOG_H

#include <QDialog>
#include <QToolButton>

namespace Ui {
class DriverColorsDialog;
}

class DriverColorsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DriverColorsDialog(QWidget *parent = 0);
    ~DriverColorsDialog();
    int exec();

public slots:
    void onColorButtonClicked();
    void onResetButtonClicked();
    
private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    void setButtonColor(QToolButton *button, QColor color);

    Ui::DriverColorsDialog *ui;
    QList<QColor> colors;
};

#endif // DRIVERCOLORSDIALOG_H
