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

#include "stdafx.h"
#include "AppSettings.h"

namespace
{
const wchar_t leftParam[] = L"left";
const wchar_t rightParam[] = L"right";
const wchar_t topParam[] = L"top";
const wchar_t bottomParam[] = L"bottom";
const wchar_t changeSvAttrDlgSection[] = L"ChangeSvAttrDlg";
}
void AppSettings::SaveSettings()
{
    CWinApp * app = AfxGetApp();
    // connection settings
    VERIFY(app);    
    // change attribute dialog settings    
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, leftParam, changeSvAttrDlgPos_.left));
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, topParam, changeSvAttrDlgPos_.top));
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, rightParam, changeSvAttrDlgPos_.right));
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, bottomParam, changeSvAttrDlgPos_.bottom));

    mws_.Save();
    cs_.Save();
}

void AppSettings::LoadSettings()
{
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    auto getint = [app](const wchar_t* section, const wchar_t * param, int def) {
        return (app->GetProfileInt(section, param, def));
    };
    mws_.Load();
    cs_.Load();    
    // change attribute dialog settings
    changeSvAttrDlgPos_ =
        CRect(getint(changeSvAttrDlgSection, leftParam, 0), getint(changeSvAttrDlgSection, topParam, 0),
              getint(changeSvAttrDlgSection, rightParam, 0), getint(changeSvAttrDlgSection, bottomParam, 0));
}

