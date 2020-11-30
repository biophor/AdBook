#include "stdafx.h"
#include "GdiPlusInitializer.h"

namespace qmladbook
{
    GdiPlusInitializer::GdiPlusInitializer()
    {
        Gdiplus::Status status = Gdiplus::GdiplusStartup(&_gdiplusToken, &_gdiPlusStartupInput, nullptr);
        if (status != Gdiplus::Status::Ok) {
            throw adbook::Error(L"GdiplusStartup() failed", __FUNCTIONW__);
        }
    }

    GdiPlusInitializer::~GdiPlusInitializer()
    {
        Gdiplus::GdiplusShutdown(_gdiplusToken);
    }
}

