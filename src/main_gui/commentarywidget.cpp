#include "commentarywidget.h"
#include "ui_commentarywidget.h"

#include <QScrollBar>

#include "../core/eventdata.h"

CommentaryWidget::CommentaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentaryWidget)
{
    ui->setupUi(this);
}

CommentaryWidget::~CommentaryWidget()
{
    delete ui;
}

void CommentaryWidget::saveSettings(QSettings &settings)
{
    settings.setValue("ui/commentary_autoscroll", ui->autoScrollBox->isChecked());
}

void CommentaryWidget::loadSettings(QSettings &settings)
{
    ui->autoScrollBox->setChecked(settings.value("ui/commentary_autoscroll", true).toBool());
}

void CommentaryWidget::setFont(const QFont &font)
{
    ui->commentaryEdit->setFont(font);
}

QString CommentaryWidget::getCommentary()
{
    return ui->commentaryEdit->toPlainText();
}

void CommentaryWidget::clear()
{
    ui->commentaryEdit->clear();
}

void CommentaryWidget::update()
{
    int position = ui->commentaryEdit->verticalScrollBar()->sliderPosition();
    ui->commentaryEdit->setText(EventData::getInstance().getCommentary());

    if (ui->autoScrollBox->isChecked())
    {
        QTextCursor c = ui->commentaryEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->commentaryEdit->setTextCursor(c);
    }
    else
    {
        ui->commentaryEdit->verticalScrollBar()->setSliderPosition(position);
    }

}
