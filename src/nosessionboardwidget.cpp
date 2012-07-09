#include "nosessionboardwidget.h"
#include "ltdata.h"

NoSessionBoardWidget::NoSessionBoardWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

NoSessionBoardWidget::~NoSessionBoardWidget()
{

}

void NoSessionBoardWidget::showSessionBoard(QString msg)
{
	int year = 2000 + msg.left(2).toInt();
	int month = msg.mid(2, 2).toInt();
	int day = msg.mid(4, 2).toInt();
	int hour = msg.mid(6, 2).toInt() + 1;

	LTEvent event = LTData::getEvent(QDate::currentDate());
	QString str = event.eventName +
		"\n\n" + QString::number(year) + "." + (month < 10 ? "0" + QString::number(month) : QString::number(month)) + "." +
		(day < 10 ? "0" + QString::number(day) : QString::number(day)) + " - " + QString::number(hour) + ":00 GMT";

	ui.sessionLabel->setText(str);
	QPixmap pix = event.trackImg.height() > 600 ? event.trackImg.scaledToHeight(600, Qt::SmoothTransformation) : event.trackImg;
	ui.trackMapLabel->setPixmap(pix);

}
