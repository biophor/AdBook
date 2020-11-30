#pragma once

#include "ADataExchanger.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
class QMLADBOOKBL_EXPORT DataExchanger: public ADataExchanger
{
public:
    void Set(SenderId sender, const QString & data) override;
    bool Get(SenderId sender, QString & data) override;

private:
    std::map<SenderId, std::any> _data;
};

}