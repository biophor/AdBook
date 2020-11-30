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

#include "stdafx.h"
#include "ContactUpdater.h"


namespace qmladbook
{
    ContactUpdater::ContactUpdater (
        std::shared_ptr<adbook::AbstractAdAccessFactory> adAccessFactory,
        std::shared_ptr< ASettingsKeeper> settingsKeeper,
        std::shared_ptr<AFileSystemService> fileSystemService
    )
        :
        _adAccessFactory{ adAccessFactory },
        _settingsKeeper{ settingsKeeper },
        _fileSystemService{ fileSystemService }
    { }

    void ContactUpdater::Update (
        Contact & contact,
        adbook::Attributes::AttrId attrId,
        const QString & newValue
    )
    {
        if (adbook::Attributes::Dn == attrId) {
            throw adbook::HrError(E_INVALIDARG, L"DN", __FUNCTIONW__);
        }
        _attributeValidator.CheckBaseConstraints(attrId, newValue);

        if (adbook::Attributes::CommonName == attrId) {
            Rename(contact, newValue);
            return;
        }
        const QString contactDn = GetDN(contact);
        auto connector = Connect(contactDn);

        auto currentLocalValue = contact.GetAttr(attrId);
        auto currentRemoteValue = Download(connector, attrId);

        if (currentLocalValue != currentRemoteValue)
        {
            Download(connector, contact);
            throw ObsoleteAttributeValue(__FUNCTIONW__);
        }

        Upload(connector, attrId, newValue);
        contact.SetAttr(attrId, newValue);
    }

    void ContactUpdater::Rename (
        Contact & contact,
        const QString & newCN
    )
    {
        const QString contactDn = GetDN(contact);
        auto connector = Connect(contactDn);
        connector->Rename(newCN.toStdWString());
        const QString newContactDn = DownloadAsString(connector, adbook::Attributes::Dn);
        contact.SetAttr(adbook::Attributes::Dn, newContactDn);
        contact.SetAttr(adbook::Attributes::CommonName, newCN);
    }

    QString ContactUpdater::GetDN (
        const Contact & contact
    )
    {
        if (const QString dn = contact.GetDn().trimmed(); dn.isEmpty()) {
            throw adbook::HrError(E_INVALIDARG, L"Dn is empty", __FUNCTIONW__);
        }
        else {
            return dn;
        }
    }

    std::unique_ptr<adbook::AbstractAdConnector> ContactUpdater::Connect (
        const QString & dn
    )
    {
        if (dn.isEmpty() || (dn != dn.trimmed())) {
            throw adbook::HrError(E_INVALIDARG, L"dn", __FUNCTIONW__);
        }
        auto connector = _adAccessFactory->CreateConnector();
        connector->Connect(_settingsKeeper->GetConnectionSettings(), dn.toStdWString());
        return connector;
    }

    AttributeValue ContactUpdater::Download (
        const QString & dn,
        adbook::Attributes::AttrId attrId
    )
    {
        auto connector = Connect(dn);
        return Download(connector, attrId);
    }

    AttributeValue ContactUpdater::Download (
        std::unique_ptr<adbook::AbstractAdConnector> & connector,
        adbook::Attributes::AttrId attrId
    )
    {
        auto & attrTraits = adbook::Attributes::GetInstance();
        const std::wstring ldapAttrName = attrTraits.GetLdapAttrName(attrId);

        AttributeValue attrValue;
        if (attrTraits.IsString(attrId)) {
            const std::wstring stringValue = connector->DownloadStringAttr(ldapAttrName);
            attrValue = QString::fromStdWString(stringValue);
        }
        else {
            const adbook::BinaryAttrVal binaryValue = connector->DownloadBinaryAttr(ldapAttrName);
            attrValue = binaryValue;
        }
        return attrValue;
    }

    QString ContactUpdater::DownloadAsString (
        std::unique_ptr<adbook::AbstractAdConnector> & connector,
        adbook::Attributes::AttrId attrId
    )
    {
        return std::get<QString>(Download(connector, attrId));
    }

    adbook::BinaryAttrVal ContactUpdater::DownloadAsBinary (
        std::unique_ptr<adbook::AbstractAdConnector> & connector,
        adbook::Attributes::AttrId attrId
    )
    {
        return std::get<adbook::BinaryAttrVal>(Download(connector, attrId));
    }

    void ContactUpdater::Upload (
        const QString & dn,
        adbook::Attributes::AttrId attrId,
        const AttributeValue & attrValue
    )
    {
        auto connector = Connect(dn);
        Upload(connector, attrId, attrValue);
    }

    void ContactUpdater::Upload (
        std::unique_ptr<adbook::AbstractAdConnector> & connector,
        adbook::Attributes::AttrId attrId,
        const AttributeValue & attrValue
    )
    {
        auto & attrTraits = adbook::Attributes::GetInstance();
        const std::wstring ldapAttrName = attrTraits.GetLdapAttrName(attrId);

        if (attrTraits.IsString(attrId)) {
            const std::wstring stringAttrValue = std::get<QString>(attrValue).toStdWString();
            connector->UploadStringAttr(ldapAttrName, stringAttrValue);
        }
        else {
            const adbook::BinaryAttrVal binaryAttrValue = std::get<adbook::BinaryAttrVal>(attrValue);
            connector->UploadBinaryAttr(ldapAttrName, binaryAttrValue);
        }
    }

    void ContactUpdater::UpdatePhoto (
        Contact & contact,
        const QByteArray & photo
    )
    {
        _attributeValidator.CheckBaseConstraints(adbook::Attributes::ThumbnailPhoto, photo);

        const QString contactDn = GetDN(contact);
        auto connector = Connect(contactDn);

        auto currentLocalValue = contact.GetAttr(adbook::Attributes::ThumbnailPhoto);
        auto currentRemoteValue = Download(connector, adbook::Attributes::ThumbnailPhoto);

        if (currentLocalValue != currentRemoteValue)
        {
            Download(connector, contact);
            throw ObsoleteAttributeValue(__FUNCTIONW__);
        }
        const AttributeValue newAttrValue = AttributeValueFromQByteArray(photo);

        Upload(connector, adbook::Attributes::ThumbnailPhoto, newAttrValue);
        contact.SetAttr(adbook::Attributes::ThumbnailPhoto, newAttrValue);
    }

    void ContactUpdater::UpdatePhoto (
        Contact & contact,
        const QString & filePath
    )
    {
        if (!_fileSystemService->IsJpegFile(filePath)) {
            throw adbook::HrError(E_INVALIDARG, L"not a jpeg-file", __FUNCTIONW__);
        }
        const QByteArray fileBytes = _fileSystemService->ReadFile(filePath);
        UpdatePhoto(contact, fileBytes);
    }

    void ContactUpdater::ClearPhoto (
        Contact & contact
    )
    {
        UpdatePhoto(contact, QByteArray{});
    }

    void ContactUpdater::Download (
        const QString & contactDn,
        Contact & contact
    )
    {
        auto connector = Connect(contactDn);
        Download(connector, contact);
    }

    void ContactUpdater::Download (
        std::unique_ptr<adbook::AbstractAdConnector> & connector,
        Contact & contact
    )
    {
        for (auto & attrId : adbook::Attributes::GetInstance().GetAttrIds()) {
            contact.SetAttr(attrId, Download(connector, attrId));
        }
    }

    AttributeValue ContactUpdater::AttributeValueFromQByteArray(
        const QByteArray & source
    )
    {
        AttributeValue ret;
        if (source.isEmpty()) {
            return ret;
        }
        adbook::BinaryAttrVal dest(source.size());
        memcpy_s(dest.data(), dest.size(), source.data(), static_cast<size_t>(source.length()));
        return dest;
    }
}

