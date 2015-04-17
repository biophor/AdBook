#pragma once
#include "AdBookBLExport.h"
#include "AdPersonDesc.h"

namespace adbook
{

class ADBOOKBL_API AdPersonDescList : public std::deque<AdPersonDesc>
{
public:
    void Load();
    void Save();

private:
    void InternalLoad();
    void InternalSave();
};

}   // namespace adbook
