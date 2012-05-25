#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSettings>

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

    void setAlternatingRowColors(bool);
    bool isAlternatingRowColors();

    void setSplitterOpaqueResize(bool);
    bool isSplitterOpaqueResize();

    void setDrawCarThumbnails(bool);
    bool drawCarThumbnails();

    void setReverseOrderLapHistory(bool);
    bool isReverseOrderLapHistory();

    void setReverseOrderHeadToHead(bool);
    bool isReverseOrderHeadToHead();

    void setReverseOrderLapTimeComparison(bool);
    bool isReverseOrderLapTimeComparison();

    void setAutoRecord(bool);
    bool isAutoRecord();

public slots:
    int exec(QSettings *);
    
private slots:
    void on_buttonBox_accepted();

    void on_mainFontButton_clicked();

    void on_commentaryFontButton_clicked();

private:
    Ui::PreferencesDialog *ui;

    QFont mainFont;
    QFont commentaryFont;
    QSettings *settings;
};

#endif // PREFERENCESDIALOG_H
