#ifndef LTTABLEVIEW_H
#define LTTABLEVIEW_H

#include <QTableView>

class LTTableView : public QTableView
{
    Q_OBJECT
public:
    explicit LTTableView(QWidget *parent = 0);
    
signals:
    void headerClicked(int column);
    
public slots:
    void updateColumn(QModelIndex index);

protected:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    
};

#endif // LTTABLEVIEW_H
