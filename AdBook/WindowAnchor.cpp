// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "WindowAnchor.h"


void WindowAnchor::Init(HWND wndHandle)
{
    if (!IsWindow(wndHandle))
    {
        HR_ERROR(E_INVALIDARG);
    }
    wndHandle_ = wndHandle;
    if (!GetClientRect(wndHandle_, &rect_))
    {
        HR_ERROR(E_UNEXPECTED);
    }
    initRect_ = rect_;
}

void WindowAnchor::Add(UINT id, UINT flags, float cxLeft, float cxRight, float cyTop, float cyBottom)
{
    CRect rect;
    HWND wndHandle = GetDlgItem(wndHandle_, id);
    if (nullptr == wndHandle)
    {
        HR_ERROR(E_INVALIDARG);
    }
    if (GetWindowRect(wndHandle, &rect))
    {
        ScreenToClient(wndHandle_, &rect.TopLeft());
        ScreenToClient(wndHandle_, &rect.BottomRight());
        WndItem wi(wndHandle, flags, &rect, cxLeft, cxRight, cyTop, cyBottom);
        wndItems_[id] = wi;
    }
}

BOOL WindowAnchor::OnSize()
{
    if (wndItems_.empty())
    {
        return FALSE;
    }
    CRect rect;
    HDWP hdwp = BeginDeferWindowPos(static_cast<int>(wndItems_.size()));
    if (FALSE == GetClientRect(wndHandle_, &rect))
    {
        return FALSE;
    }

    for (auto & a: wndItems_)
    {
        WndItem & item = a.second;
        // horizontal
        if ((item.flags_ & Left) && (item.flags_ & Right))
        {
            const LONG wndDiff = rect.right - initRect_.right;
            const LONG ctlRDiff = static_cast<LONG>(ceilf(wndDiff * item.cxRight_));
            item.rect_.right = item.initRect_.right + ctlRDiff;
            const LONG ctlLDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item.cxLeft_));
            item.rect_.left = item.initRect_.left + ctlLDiff;
        }
        else if (item.flags_ & Left)
        {
            const LONG wndDiff = rect.right - initRect_.right;
            const LONG ctlDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item.cxLeft_));
            item.rect_.left = item.initRect_.left + ctlDiff;
        }
        else if (item.flags_ & Right)
        {
            const LONG wndDiff = rect.right - initRect_.right;
            const LONG ctlDiff = static_cast<LONG>(ceilf(wndDiff * item.cxRight_));
            item.rect_.right = item.initRect_.right + ctlDiff;
            item.rect_.left = item.initRect_.left + ctlDiff;
        }
        else
        {
            // relative move
            const LONG sx = ((rect.right - rect.left) / 2) - ((rect_.right - rect_.left) / 2);
            item.rect_.right += sx;
            item.rect_.left += sx;
        }
        // vertical
        if ((item.flags_ & Top) && (item.flags_ & Bottom))
        {
            const LONG wndDiff = rect.bottom - initRect_.bottom;
            const LONG ctlBDiff = static_cast<LONG>(ceilf(wndDiff * item.cyBottom_));
            item.rect_.bottom = item.initRect_.bottom + ctlBDiff;

            const LONG ctlTDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item.cyTop_));
            item.rect_.top = item.initRect_.top + ctlTDiff;
        }
        else if (item.flags_ & Top)
        {
            const LONG wndDiff = rect.bottom - initRect_.bottom;
            const LONG ctlDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item.cyTop_));
            item.rect_.top = item.initRect_.top + ctlDiff;
        }
        else if (item.flags_ & Bottom)
        {
            const LONG wndDiff = rect.bottom - initRect_.bottom;
            const LONG ctlDiff = static_cast<LONG>(ceilf(wndDiff * item.cyBottom_));
            item.rect_.bottom = item.initRect_.bottom + ctlDiff;
            item.rect_.top = item.initRect_.top + ctlDiff;
        }
        else
        {
            // relative move
            const LONG sy = ((rect.bottom - rect.top) / 2) - ((rect_.bottom - rect_.top) / 2);
            item.rect_.bottom += sy;
            item.rect_.top += sy;
        }

        DeferWindowPos( hdwp, item.wndHandle_, nullptr, item.rect_.left, item.rect_.top,
            item.rect_.right - item.rect_.left, item.rect_.bottom - item.rect_.top,
            SWP_NOZORDER );
    }
    rect_ = rect;
    EndDeferWindowPos(hdwp);
    return TRUE;
}

