#include "stdafx.h"
#include "AutoToolTipDelegate.h"

AutoToolTipDelegate::AutoToolTipDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

AutoToolTipDelegate::~AutoToolTipDelegate()
{

}


bool AutoToolTipDelegate::helpEvent(QHelpEvent * e, QAbstractItemView * view,
    const QStyleOptionViewItem & option, const QModelIndex & index)
{
    if (!e || !view)
        return false;

    if (e->type() == QEvent::ToolTip) {
        QRect rect = view->visualRect(index);
        QSize size = sizeHint(option, index);
        if (rect.width() < size.width()) {
            QVariant tooltip = index.data(Qt::DisplayRole);
            if (tooltip.canConvert<QString>()) {
                QToolTip::showText(e->globalPos(), QString("<div>%1</div>")
                    .arg(tooltip.toString()), view);
                return true;
            }
        }
        if (!QStyledItemDelegate::helpEvent(e, view, option, index))
            QToolTip::hideText();
        return true;
    }
    return QStyledItemDelegate::helpEvent(e, view, option, index);
}