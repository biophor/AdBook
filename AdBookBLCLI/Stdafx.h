// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#pragma once
#include "..\AdBookBL\export.h"

#include <boost/numeric/conversion/cast.hpp>
#include <vcclr.h>  

#include <Commctrl.h>
#include <exception>

#define FINALISER_CALL_WARNING \
    System::Diagnostics::Trace::TraceWarning(\
        "YOU FORGOT TO CALL DISPOSE!!! {0}, {1}", \
        __FUNCTION__, GetHashCode());