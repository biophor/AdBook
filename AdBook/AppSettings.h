/*
Copyright (C) 2015-2020 Goncharov Andrei.

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
#include "MainWndSettings.h"

class AppSettings
{
public:
    MainWndSettings & GetMainWndSettings() noexcept {
        return _mainWndSettings;
    }
    const MainWndSettings & GetMainWndSettings() const noexcept {
        return _mainWndSettings;
    }

    adbook::ConnectionParams & GetConnectionParams() {
        return _connectionParams;
    }

    const adbook::ConnectionParams & GetConnectionParams() const {
        return _connectionParams;
    }

    CRect GetSvAttrEditorPosition() const noexcept {
        return _svAttrEditorPosition;
    }
    void SetSvAttrEditorPosition(const CRect & rc)
    {
        if (rc.IsRectEmpty() || rc.IsRectNull())
        {
            HR_ERROR(E_INVALIDARG);
        }
        _svAttrEditorPosition = rc;
    }

private:
    CRect _svAttrEditorPosition;  // sv -> single value
    MainWndSettings _mainWndSettings;
    adbook::ConnectionParams _connectionParams;
};

