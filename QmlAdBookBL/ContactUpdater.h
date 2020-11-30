/*
Copyright (C) 2015-2021 Andrei Goncharov.

This file is part of the 'Active Directory Contact Book'.
'Active Directory Contact Book' is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

'Active Directory Contact Book' is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
'Active Directory Contact Book'. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "AContactUpdater.h"
#include "qmladbookbl_global.h"
#include "ASettingsKeeper.h"
#include "AFileSystemService.h"
#include "ContactAttributeValidator.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT ContactUpdater : public AContactUpdater
    {
    public:
        ContactUpdater(
            std::shared_ptr<adbook::AbstractAdAccessFactory> adAccessFactory,
            std::shared_ptr<ASettingsKeeper> settingsKeeper,
            std::shared_ptr<AFileSystemService> fileSystemService
            );

        void Update (
            Contact & contact,
            adbook::Attributes::AttrId attrId,
            const QString & value
        ) override;

        void UpdatePhoto (
            Contact & contact,
            const QString & filePath
        ) override;

        void ClearPhoto (
            Contact & contact
        ) override;

    private:
        std::unique_ptr<adbook::AbstractAdConnector> Connect (
            const QString & dn
        );

        AttributeValue Download (
            const QString & dn,
            adbook::Attributes::AttrId attrId
        );

        void Download (
            const QString & contactDn,
            Contact & contact
        );

        void Download (
            std::unique_ptr<adbook::AbstractAdConnector> & connector,
            Contact & contact
        );

        AttributeValue Download (
            std::unique_ptr<adbook::AbstractAdConnector> & connector,
            adbook::Attributes::AttrId attrId
        );

        QString DownloadAsString (
            std::unique_ptr<adbook::AbstractAdConnector> & connector,
            adbook::Attributes::AttrId attrId
        );

        adbook::BinaryAttrVal DownloadAsBinary (
            std::unique_ptr<adbook::AbstractAdConnector> & connector,
            adbook::Attributes::AttrId attrId
        );

        void Upload (
            const QString & dn,
            adbook::Attributes::AttrId attrId,
            const AttributeValue & attrValue
        );

        void Upload (
            std::unique_ptr<adbook::AbstractAdConnector> & connector,
            adbook::Attributes::AttrId attrId,
            const AttributeValue & attrValue
        );

        static QString GetDN (
            const Contact & contact
        );

        void Rename (
            Contact & contact,
            const QString & newCN
        );

        void UpdatePhoto (
            Contact & contact,
            const QByteArray & photo
        );

    private:
        static AttributeValue AttributeValueFromQByteArray(
            const QByteArray & data
        );
    private:
        std::shared_ptr<adbook::AbstractAdAccessFactory> _adAccessFactory;
        std::shared_ptr<ASettingsKeeper> _settingsKeeper;
        std::shared_ptr<AFileSystemService> _fileSystemService;

        ContactAttributeValidator _attributeValidator;
    };
}

