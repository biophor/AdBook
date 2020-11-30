#pragma once
namespace qmladbook
{
    class GdiPlusInitializer final
    {
    public:
        GdiPlusInitializer();
        GdiPlusInitializer(const GdiPlusInitializer &) = delete;
        GdiPlusInitializer & operator = (const GdiPlusInitializer &) = delete;
        ~GdiPlusInitializer();

    private:
        ULONG_PTR _gdiplusToken{};
        Gdiplus::GdiplusStartupInput _gdiPlusStartupInput{};
    };
}