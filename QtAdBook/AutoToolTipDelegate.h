#ifndef AUTOTOOLTIPDELEGATE_H
#define AUTOTOOLTIPDELEGATE_H


// this class was copied from: https://api.kde.org/4.x-api/kdesdk-apidocs/kcachegrind/html/classAutoToolTipDelegate.html
class AutoToolTipDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    AutoToolTipDelegate(QObject* parent);
    ~AutoToolTipDelegate();

public slots:
    bool helpEvent(QHelpEvent* e, QAbstractItemView* view, const QStyleOptionViewItem& option,
        const QModelIndex& index);
};

#endif // AUTOTOOLTIPDELEGATE_H
