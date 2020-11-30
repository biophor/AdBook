#include "stdafx.h"
#include "AContactsModel.h"

namespace qmladbook
{
    AContactsModel::AContactsModel(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    AContactsModel::~AContactsModel() = default;
}
