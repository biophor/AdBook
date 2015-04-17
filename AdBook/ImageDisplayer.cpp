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
#include "ImageDisplayer.h"

namespace 
{
    CComQIPtr<IStream> CreateAutoReleasedStream()
    {
        CComQIPtr<IStream> streamPtr;
        HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &streamPtr);
        if (hr != S_OK)
        {
            HR_ERROR(hr);
        }
        return streamPtr;
    }
}

void ImageDisplayer::Load(const std::vector<BYTE> & image)
{
    if (image.empty())
    {
        HR_ERROR(E_INVALIDARG);
    }
    Unload();
    streamPtr_ = CreateAutoReleasedStream();
    ULONG numWritten = 0;
    const HRESULT hr = streamPtr_->Write(&image.at(0), static_cast<ULONG>(image.size()), &numWritten);
    if(hr != S_OK)
    {
        HR_ERROR(hr);
    }
    if (numWritten != image.size())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    Invalidate();
    RedrawWindow();
}

void ImageDisplayer::Unload()
{	
    streamPtr_.Release();
}

void ImageDisplayer::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();
    ModifyStyle(0, SS_OWNERDRAW);
}

namespace
{
    bool ScaleImageToRect(const UINT imageWidth, const UINT imageHeight, const CRect & wndRect, CRect & imageRect)
    {
        imageRect = wndRect;
        const int wndW = wndRect.Width();
        const int wndH = wndRect.Height();
        if (0 == wndW || 0 ==wndH || 0==imageWidth || 0==imageHeight)
        {
            return false;
        }
        const double kw = static_cast<double>(imageWidth) / wndW;
        const double kh = static_cast<double>(imageHeight) / wndH;
        const double kmax = max(kw, kh);
        imageRect.right = imageRect.left + static_cast<int>(imageWidth / kmax);
        imageRect.bottom = imageRect.top + static_cast<int>(imageHeight / kmax);
        int ptX = imageRect.left;
        int ptY = imageRect.top;
        if (imageRect.Width() < wndW)
        {
            ptX += (wndW - imageRect.Width()) / 2;
        }
        if (imageRect.Height() < wndH)
        {
            ptY += (wndH - imageRect.Height()) / 2;
        }
        imageRect.MoveToXY(ptX, ptY);
        return true;
    }
}

void ImageDisplayer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CRect rc;
    GetClientRect(&rc);
    Gdiplus::Graphics graphics(lpDrawItemStruct->hDC);
    if(streamPtr_ != nullptr)
    {
        Gdiplus::Image image(streamPtr_);
        CRect imageRect;
        if (ScaleImageToRect(image.GetWidth(), image.GetHeight(), rc, imageRect))
        {
            Gdiplus::Color c;
            c.SetFromCOLORREF(GetBkColor(lpDrawItemStruct->hDC));
            graphics.Clear(c);
            graphics.DrawImage(&image, imageRect.left, imageRect.top, imageRect.Width(), imageRect.Height());
            return;
        }
    }
    Gdiplus::Color c;
    c.SetFromCOLORREF(GetBkColor(lpDrawItemStruct->hDC));
    graphics.Clear(c);
}

