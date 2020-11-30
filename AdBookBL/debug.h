#pragma once

#ifdef _DEBUG
#define MY_ASSERT _ASSERTE
#define MY_VERIFY MY_ASSERT
#define MY_VERIFY_(result, expression) MY_ASSERT(result == expression)
#else
#define MY_ASSERT __noop
#define MY_VERIFY(expression) (expression)
#define MY_VERIFY_(result, expression) (expression)
#endif

#ifdef _DEBUG
inline auto Trace(wchar_t const * format, ...) -> void
{
    va_list args;
    va_start(args, format);

    wchar_t buffer[256];

    MY_ASSERT(-1 != _vsnwprintf_s(buffer,
        _countof(buffer) - 1,
        format,
        args));

    va_end(args);

    OutputDebugString(buffer);
}

struct Tracer
{
    char const * _filename;
    unsigned _line;

    Tracer(char const * filename, unsigned const line) :
        _filename{ filename },
        _line{ line }
    {

    }

    template <typename... Args>
    void operator()(wchar_t const * format, Args... args) const
    {
        wchar_t buffer[512];

        auto count = swprintf_s(buffer,
            L"%S(%d): ",
            _filename,
            _line);

        MY_ASSERT(-1 != count);

        MY_ASSERT(-1 != _snwprintf_s(buffer + count,
            _countof(buffer) - count,
            _countof(buffer) - count - 1,
            format,
            args...));

        OutputDebugStringW(buffer);
    }

    template <typename... Args>
    void operator()(char const * format, Args... args) const
    {
        char buffer[512];

        auto count = sprintf_s(buffer,
            "%s(%d): ",
            _filename,
            _line);

        MY_ASSERT(-1 != count);

        MY_ASSERT(-1 != _snprintf_s(buffer + count,
            _countof(buffer) - count,
            _countof(buffer) - count - 1,
            format,
            args...));

        OutputDebugStringA(buffer);
    }
};

#endif

#ifdef _DEBUG
#define MY_TRACE Tracer(__FILE__, __LINE__)
#else
#define MY_TRACE __noop
#endif

#define MY_TRACE_STDEXCEPTION(e) MY_TRACE("Error: what:%s", e.what());
