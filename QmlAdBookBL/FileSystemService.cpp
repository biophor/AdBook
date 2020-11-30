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
#include "FileSystemService.h"

namespace qmladbook
{
    QByteArray FileSystemService::ReadFile (
        const QString & filePath
    )
    {
        if (filePath.isEmpty()) {
            throw adbook::HrError(E_INVALIDARG, L"filePath is empty", __FUNCTIONW__);
        }

        QString normalizedFilePath = filePath;

        if (filePath.startsWith("file:///", Qt::CaseInsensitive)) {
            normalizedFilePath = QUrl(filePath).toLocalFile();
        }
        if (!QFile::exists(normalizedFilePath)) {
            throw adbook::HrError(E_INVALIDARG, L"The file could't be found.", __FUNCTIONW__);
        }
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            wchar_t errorBuf[256]= L"Failed to open the file: ";
            StringCchCatW(errorBuf, std::size(errorBuf), filePath.toStdWString().c_str());
            throw adbook::HrError(E_INVALIDARG, errorBuf, __FUNCTIONW__);
        }
        return file.readAll();
    }

    bool FileSystemService::IsJpegFile (
        const QString & filePath
    )
    {
        if (!QFileInfo::exists(filePath)) {
            throw adbook::Error(L"file not found.", __FUNCTIONW__);
        }
        std::unique_ptr<Gdiplus::Image> imagePtr(Gdiplus::Image::FromFile(filePath.toStdWString().c_str()));
        if (!imagePtr) {
            throw adbook::Error(L"Failed to open the image.", __FUNCTIONW__);
        }
        GUID imageFormatGuid{};
        Gdiplus::Status status = imagePtr->GetRawFormat(&imageFormatGuid);
        if (status != Gdiplus::Status::Ok) {
            throw adbook::Error(L"Failed to detemine a file format.");
        }
        return Gdiplus::ImageFormatJPEG == imageFormatGuid;
    }

    __int64 FileSystemService::GetFileSize (
        const QString & filePath
    )
    {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists()) {
            throw adbook::Error(L"file not found", __FUNCTIONW__);
        }
        return fileInfo.size();
    }
}

