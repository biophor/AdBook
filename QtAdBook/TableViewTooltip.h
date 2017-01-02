#ifndef TABLEVIEWTOOLTIP_H
#define TABLEVIEWTOOLTIP_H

#include <QObject>

class TableViewTooltip : public QObject
{
    Q_OBJECT
public:
    TableViewTooltip(QAbstractItemView *parent);
    ~TableViewTooltip();
protected:
    bool eventFilter(QObject * obj, QEvent * event);    
};

#endif // TABLEVIEWTOOLTIP_H
