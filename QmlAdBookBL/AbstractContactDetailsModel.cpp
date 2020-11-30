#include "stdafx.h"
#include "AbstractContactDetailsModel.h"

namespace qmladbook
{
    AbstractContactDetailsModel::AbstractContactDetailsModel(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    AbstractContactDetailsModel::~AbstractContactDetailsModel() = default;
}