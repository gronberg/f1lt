#ifndef COMMENTARYWIDGET_H
#define COMMENTARYWIDGET_H

#include <QSettings>
#include <QWidget>

namespace Ui {
class CommentaryWidget;
}

class CommentaryWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CommentaryWidget(QWidget *parent = 0);
    ~CommentaryWidget();

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);

    QString getCommentary();
    void clear();

public slots:
    void update();
    
private:
    Ui::CommentaryWidget *ui;
};

#endif // COMMENTARYWIDGET_H
