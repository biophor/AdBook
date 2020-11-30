#include "stdafx.h"
#include "AbstractSearchFilterRulesModel.h"
namespace qmladbook
{
    AbstractSearchFilterRulesModel::AbstractSearchFilterRulesModel(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    AbstractSearchFilterRulesModel::~AbstractSearchFilterRulesModel() = default;
}
