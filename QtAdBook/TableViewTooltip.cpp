#include "stdafx.h"
#include "TableViewTooltip.h"

TableViewTooltip::TableViewTooltip(QAbstractItemView * parent)
    : QObject(parent)
{

}

TableViewTooltip::~TableViewTooltip()
{

}

bool TableViewTooltip::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() != QEvent::ToolTip) {
        return false;
    }
    QAbstractItemView* view = qobject_cast<QAbstractItemView*>(obj->parent());
    if (!view) {
        return false;
    }
    
    QHelpEvent * helpEvent = static_cast<QHelpEvent*>(event);
    QPoint pos = helpEvent->pos();
    QModelIndex index = view->indexAt(pos);
    if (!index.isValid()) {
        return false;
    }
    QString itemTooltip = view->model()->data(index, Qt::ToolTipRole).toString();
    if (itemTooltip.isEmpty()) {
        QToolTip::hideText();
        return true;
    }
    QString itemText = view->model()->data(index).toString();
    
    QFontMetrics fm(view->font());
    int textIdentWidth =6;// fm.width('r'); // don't know how to calculate an ident for text in an item :-( 
    int itemTextWidth = fm.width(itemText) + textIdentWidth;
    QRect rect = view->visualRect(index);
    
    int rectWidth = rect.width();

    if (itemTextWidth > rectWidth) {
        QToolTip::showText(helpEvent->globalPos(), itemTooltip, view, rect);
    }
    else {
        QToolTip::hideText();
    }
    return true;    
}

