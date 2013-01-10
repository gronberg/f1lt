#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QNetworkProxy>
#include <QSettings>
#include <QToolButton>

#include "drivercolorsdialog.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

    void setFonts(const QFont &f1, const QFont &f2);

    void setMainFont(const QFont &f);
    void setCommentaryFont(const QFont &f);

    QFont getMainFont() { return mainFont; }
    QFont getCommentaryFont() { return commentaryFont; }

    void setSplitterOpaqueResize(bool);
    bool isSplitterOpaqueResize();

    void setDrawCarThumbnails(bool);
    bool drawCarThumbnails();

    void setAutoRecord(bool);
    bool isAutoRecord();

    void setAutoStopRecord(int);
    int getAutoStopRecord();

    void setAutoConnect(bool);
    bool isAutoConnect();

    bool drawTrackerClassification();
    void setDrawTrackerClassification(bool);

    QNetworkProxy getProxy();
    bool proxyEnabled();

public slots:
    int exec(QSettings *);
    
private slots:
    void on_buttonBox_accepted();

    void on_mainFontButton_clicked();

    void on_commentaryFontButton_clicked();

    void on_autoStopRecordBox_toggled(bool checked);

    void on_proxyCheckBox_toggled(bool checked);

    void on_pushButton_clicked();

    void on_colorWhiteButton_clicked();

    void on_colorCyanButton_clicked();

    void on_colorYellowButton_clicked();

    void on_colorRedButton_clicked();

    void on_colorGreenButton_clicked();

    void on_colorVioletButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:

    void setButtonColor(QToolButton *button, QColor color);

    Ui::PreferencesDialog *ui;

    QFont mainFont;
    QFont commentaryFont;
    QSettings *settings;
    QList<QColor> colors;

    DriverColorsDialog *dcDialog;
};

#endif // PREFERENCESDIALOG_H
