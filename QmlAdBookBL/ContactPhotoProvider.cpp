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
#include "ContactPhotoProvider.h"

namespace qmladbook
{
    ContactPhotoProvider::ContactPhotoProvider() = default;

    ContactPhotoProvider::~ContactPhotoProvider() = default;

    QString ContactPhotoProvider::SetContactPhoto(const QString & id, const std::vector<BYTE> & imageBytes)
    {
        if (id.isEmpty()) {
            throw adbook::HrError(E_INVALIDARG, L"id", __FUNCTIONW__);
        }
        if (imageBytes.empty()) {
            throw adbook::HrError(E_INVALIDARG, L"imageBytes is empty", __FUNCTIONW__);
        }
        if (imageBytes.size() > std::numeric_limits<uint>::max()) {
            throw adbook::HrError(E_INVALIDARG, L"imageBytes is too big", __FUNCTIONW__);
        }
        QPixmap photoPixmap;
        if (!photoPixmap.loadFromData(imageBytes.data(), static_cast<uint>(imageBytes.size()))) {
            throw adbook::HrError(E_INVALIDARG, L"imageBytes has unsupported format", __FUNCTIONW__);
        }
        _photos[id] = photoPixmap;
        return "image://" + GetProviderName() + "/" + id;
    }

    QPixmap ContactPhotoProvider::requestPixmap(const QString & id, QSize *size, const QSize & requestedSize)
    {
        QPixmap photo = GetPhoto(id);

        if (size) {
            *size = QSize(photo.width(), photo.height());
        }
        QPixmap scaledPhoto = photo.scaled(
            requestedSize.width() > 0 ? requestedSize.width() : photo.width(),
            requestedSize.height() > 0 ? requestedSize.height() : photo.height(),
            Qt::KeepAspectRatio
        );
        return scaledPhoto;
    }

    QPixmap ContactPhotoProvider::GetPhoto(const QString & id)
    {
        if (auto iter = _photos.find(id); iter != _photos.cend())
        {
            return iter->copy();
        }
        if (_noPhotoReplacement.isNull())
        {
            CreateNoPhotoReplacement();
        }
        return _noPhotoReplacement.copy();
    }

    void ContactPhotoProvider::CreateNoPhotoReplacement()
    {
        QPixmap pixmap(100, 50);
        pixmap.fill(QColor(Qt::gray).rgba());
        QPainter painter(&pixmap);
        QFont f = painter.font();
        painter.setFont(f);
        painter.setPen(Qt::black);
        painter.drawText(QRectF(0, 0, pixmap.width(), pixmap.height()),
            Qt::AlignCenter, QObject::tr("There is no photo"));
        _noPhotoReplacement = pixmap;
    }

    QString ContactPhotoProvider::GetProviderName() const
    {
        return "ContactPhotoProvider";
    }

    void ContactPhotoProvider::Clear()
    {
        _photos.clear();
    }
}
