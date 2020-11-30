#include "stdafx.h"
#include "DataExchanger.h"

namespace qmladbook
{
    void DataExchanger::Set(SenderId sender, const QString & data)
    {
        _data[sender] = data;
    }

    bool DataExchanger::Get(SenderId sender, QString & data)
    {
        if (_data.find(sender) == _data.cend()) {
            return false;
        }
        data = std::any_cast<QString>(_data[sender]);
        return true;
    }
}
