#include "stdafx.h"
#include "AttrTraits.h"

namespace qmladbook
{
    AttrTraits & AttrTraits::Instance() {
        static AttrTraits attrTraits;
        return attrTraits;
    }

    unsigned int AttrTraits::GetMaxSizeInBytes(const adbook::Attributes::AttrId id) {
        return static_cast<unsigned int>(adAttr.GetAttrMaxLength(id));
    }

    QString AttrTraits::GetLdapAttrName(const adbook::Attributes::AttrId id) {
        return QString::fromWCharArray(adAttr.GetLdapAttrNamePtr(id));
    }

    QString AttrTraits::GetAttrOID(const adbook::Attributes::AttrId id) {
        return QString::fromStdWString(adAttr.GetAttrOid(id));
    }
}

