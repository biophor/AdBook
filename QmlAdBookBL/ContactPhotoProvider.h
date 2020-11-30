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

#include "AContactPhotoProvider.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT ContactPhotoProvider : public AContactPhotoProvider
    {
    public:
        ContactPhotoProvider();
        virtual ~ContactPhotoProvider();

        QString SetContactPhoto(const QString & id, const std::vector<BYTE> & imageBytes) override;
        QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
        QString GetProviderName() const override;
        void Clear() override;
    private:
        QPixmap GetPhoto(const QString & id);
        void CreateNoPhotoReplacement();
    private:
        QPixmap _noPhotoReplacement;
        QHash<QString, QPixmap> _photos;
    };
}
