// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
