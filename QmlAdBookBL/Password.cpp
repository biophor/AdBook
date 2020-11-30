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
#include "Password.h"

namespace qmladbook
{
    Password::Password(const Password & that) = default;

    Password & Password::operator = (const Password & that) = default;

    bool Password::TryToSet(const QString & plainTextPassword)
    {
        QByteArray utf8Password = plainTextPassword.toUtf8();
        DATA_BLOB db{};
        BYTE dummy[1]{};
        if (plainTextPassword.isEmpty()) {
            db.pbData = dummy;
            db.cbData = 0;
        }
        else {
            db.cbData = static_cast<DWORD>(utf8Password.size());
            db.pbData = reinterpret_cast<BYTE*>(utf8Password.data());
        }
        DATA_BLOB rdb{};
        if (!CryptProtectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &rdb)) {
            return false;
        }
        _encodedData = std::vector<BYTE>(rdb.pbData, rdb.pbData + rdb.cbData);
        LocalFree(rdb.pbData);
        return true;
    }

    QByteArray ToQByteArray(const std::vector<BYTE> & v) {
        if (v.empty()) {
            return {};
        }
        return QByteArray(
            reinterpret_cast<const char *>(v.data()),
            static_cast<int>(v.size())
        );
    }

    QByteArray ToQByteArray(const DATA_BLOB & db) {
        if (db.cbData == 0 || db.pbData == nullptr) {
            return {};
        }
        return QByteArray(
            reinterpret_cast<const char *>(db.pbData),
            static_cast<int>(db.cbData)
        );
    }

    std::vector<BYTE> ToByteVector(const QByteArray & ba) {
        if (ba.isEmpty()) {
            return {};
        }
        return std::vector<BYTE>(
            reinterpret_cast<const BYTE *>(ba.data()),
            reinterpret_cast<const BYTE *>(ba.data() + ba.size())
            );
    }

    bool Password::TryToGetAsPlainTextString(QString & plainTextPassword) const
    {
        if (_encodedData.empty()) {
            plainTextPassword.clear();
            return false;
        }
        auto encodedData = _encodedData;
        DATA_BLOB db{};
        db.pbData = encodedData.data();
        db.cbData = static_cast<DWORD>(encodedData.size());
        DATA_BLOB rdb{};
        if (!CryptUnprotectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &rdb)) {
            return false;
        }
        QByteArray utf8Password = ToQByteArray(rdb);
        plainTextPassword = QString::fromUtf8(utf8Password);
        return true;
    }

    bool Password::IsEmpty() const {
        return _encodedData.empty();
    }

    QString Password::Serialize() const
    {
        if (_encodedData.empty()) {
            return QString();
        }
        return ToQByteArray(_encodedData).toBase64();
    }

    void Password::Deserialize(const QString & serializedPassword)
    {
        if (serializedPassword.isEmpty()) {
            _encodedData.clear();
            return;
        }
        _encodedData = ToByteVector(QByteArray::fromBase64(serializedPassword.toLatin1()));
    }
}
