#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QObject>

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

class ClickedLabel :public QLabel
{
    Q_OBJECT
public:
    explicit ClickedLabel(QWidget* parent = nullptr);
    virtual ~ClickedLabel();

signals:
    void clicked(QPoint pos);

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKEDLABEL_H
