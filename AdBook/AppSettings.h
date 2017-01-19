// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015 Goncharov Andrei.

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
#include "ConnectionSettings.h"
#include "MainWndSettings.h"

class AppSettings
{
public:    
    MainWndSettings & GetMainWndSettings() {
        return mws_;
    }
    const MainWndSettings & GetMainWndSettings() const {
        return mws_;
    }
    ConnectionSettings & GetConnectionSettings() {
        return cs_;
    }
    const ConnectionSettings & GetConnectionSettings() const {
        return cs_;
    }

    CRect GetSvChangeAttrDlgPos() const {
        return changeSvAttrDlgPos_;
    }
    void SetChangeSvAttrDlgPos(const CRect & rc)
    {
        if (rc.IsRectEmpty() || rc.IsRectNull())
        {
            HR_ERROR(E_INVALIDARG);
        }
        changeSvAttrDlgPos_ = rc;
    }
    void SaveSettings();
    void LoadSettings();

private:
    CRect changeSvAttrDlgPos_;  // sv -> single value
    ConnectionSettings cs_;
    MainWndSettings mws_;
};

