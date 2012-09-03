#ifndef SESSIONANALYSISWIDGET_H
#define SESSIONANALYSISWIDGET_H

#include <QtGui/QWidget>
#include <QIcon>
#include <QPair>
#include <QSettings>

#include "ui_sessionanalysiswidget.h"

#include "../core/eventdata.h"

class SessionAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    SessionAnalysisWidget(QWidget *parent = 0);
    ~SessionAnalysisWidget();

    void setupTables();
    void resizeTables();

protected:

    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);


public slots:
	void selectDriversClicked();
    void exec();

    void update(bool repaintCharts = true);
    void gatherData();
    bool driverChecked(int no);
    void setDriverChecked(int no, bool checked);
    void onZoomChanged(int, int, double, double);
    void setupBoxes();
    void setupIcons(QList<QColor> colors);
    QIcon getDriverIcon(int no);

    void resetView();

    bool lapInWindow(int i);

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void onSplitterMoved(int pos, int index);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_2_clicked();

    void on_top10pushButton_clicked();

    void on_qualiTabWidget_currentChanged(int index);

private:

    QTableWidgetItem* setItem(QTableWidget *table, int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                     QColor textColor = SeasonData::getInstance().getColor(LTPackets::DEFAULT), QBrush background = QBrush());

    Ui::SessionAnalysisWidgetClass ui;
    QList<QCheckBox*> driverCheckBoxes;
    QList<QLabel*> driverLabels;

    bool selected;
    int first, last;
    double min, max;
    QList<LapData> lapDataArray;
    QList< QPair<int, QIcon> > driverIcons;
    bool top10only;
};

#endif // SESSIONANALYSISWIDGET_H
