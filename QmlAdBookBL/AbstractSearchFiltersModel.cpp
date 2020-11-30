#include "stdafx.h"
#include "AbstractSearchFiltersModel.h"
namespace qmladbook
{
    AbstractSearchFiltersModel::AbstractSearchFiltersModel(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    AbstractSearchFiltersModel::~AbstractSearchFiltersModel() = default;
}
