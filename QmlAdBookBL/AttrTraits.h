#pragma once
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT AttrTraits
    {
    public:
        static AttrTraits & Instance();

        unsigned int GetMaxSizeInBytes(const adbook::Attributes::AttrId id);
        QString GetLdapAttrName(const adbook::Attributes::AttrId id);
        QString GetAttrOID(const adbook::Attributes::AttrId id);

    private:
        adbook::Attributes & adAttr{ adbook::Attributes::GetInstance() };
    };
}

