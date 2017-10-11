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

// WndItem - a helper class that is used by the class WindowAnchor.
// It repersents the position of a control in a window.
class WndItem
{
    friend class WindowAnchor;
public:
    WndItem() = default;
    WndItem(HWND hWnd, UINT uFlag, const CRect & rect, float cxLeft = 1.0f, float cxRight = 1.0f,
        float cyTop = 1.0f, float cyBottom = 1.0f
        )
        : wndHandle_(hWnd), flags_(uFlag), cxLeft_(cxLeft), cxRight_(cxRight),
        cyTop_(cyTop), cyBottom_(cyBottom), rect_(rect), initRect_(rect)
    {
    }

private:
    HWND wndHandle_ = nullptr;
    UINT flags_ = 0;
    CRect rect_;
    CRect initRect_;
    float cxLeft_ = 1.0f, cxRight_ = 1.0f, cyTop_ = 1.0f, cyBottom_ = 1.0f;
};

// WindowAnchor is used for auto correction controls' positions when a window is resising
class WindowAnchor
{
public:
    enum
    {
        Top=0x1, Left=0x2, Right=0x4, Bottom=0x8, TopLeft=0x3, TopRight=0x5, BottomLeft=0xA,
        BottomRight=0xC, All=0xF
    };
    void Init(HWND windowHandle);
    void Add(UINT id, UINT flag, float cxLeft = 1.0f, float cxRight = 1.0f, float cyTop = 1.0f,
        float cyBottom = 1.0f);
    BOOL OnSize();
private:
    HWND wndHandle_ = nullptr;
    CRect initRect_;
    CRect rect_;
    using DlgCtrlId = UINT;
    std::map<DlgCtrlId, WndItem> wndItems_;
};
