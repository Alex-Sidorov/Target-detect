#include "clickedlabel.h"

ClickedLabel::ClickedLabel(QWidget *parent):QLabel(parent)
{

}

ClickedLabel::~ClickedLabel()
{

}
void ClickedLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked(event->pos());
}
