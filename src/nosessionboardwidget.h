#ifndef NOSESSIONBOARDWIDGET_H
#define NOSESSIONBOARDWIDGET_H

#include <QtGui/QWidget>
#include "ui_nosessionboardwidget.h"

class NoSessionBoardWidget : public QWidget
{
    Q_OBJECT

public:
    NoSessionBoardWidget(QWidget *parent = 0);
    ~NoSessionBoardWidget();

    void showSessionBoard(QString msg);

private:
    Ui::NoSessionBoardWidgetClass ui;
};

#endif // NOSESSIONBOARDWIDGET_H
