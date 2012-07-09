#ifndef LTTABLEWIDGET_H
#define LTTABLEWIDGET_H

#include <QItemDelegate>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "eventdata.h"

class LTTableItemDelegate: public QItemDelegate
{
public:
    LTTableItemDelegate(QObject* parent = 0) : QItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& rOption, const QModelIndex& rIndex) const
    {
        QStyleOptionViewItem viewOption(rOption);

        QColor color = rIndex.data(Qt::ForegroundRole).value<QColor>();
        if (color.isValid())
        {
            if (color != rOption.palette.color(QPalette::WindowText))
                viewOption.palette.setColor(QPalette::HighlightedText, color);
        }
        QItemDelegate::paint(painter, viewOption, rIndex);
    }
};

class LTTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit LTTableWidget(QObject *parent = 0);

    void updateLT();


    void updateRaceEvent(int ddIdx = -1);
    void updateDriver(int driverIdx);
    void updatePracticeEvent(int ddIdx = -1);
    void updateQualiEvent(int ddIdx = -1);

    void setHeader();
    void loadCarImages();

    void setDrawCarThumbnails(bool val)
    {
        drawCarThumbnails = val;
        resizeEvent(0);
        updateLT();
    }

    bool printDiff(int row, int col);

protected:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *event);
signals:
    
public slots:

private:
    QTableWidgetItem* setItem(int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                 QColor textColor = LTData::colors[LTData::DEFAULT], QBrush background = QBrush());

    QIcon upArrowIcon;
    QIcon downArrowIcon;

    QList<QPixmap> smallCarImg;

    EventData &eventData;
    LTData::EventType eventType;

    bool drawCarThumbnails;
    int showDiff;	//1 - time (best, q1), 2 - q2, 3 - q3, 4 - interval
    int currCar;
};

#endif // LTTABLEWIDGET_H
