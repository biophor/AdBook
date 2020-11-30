// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <Shlwapi.h>
#include <atlbase.h>
#include <Shlobj.h>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <mutex>
#include <vector>
#include <atomic>
#include <future>
#include <limits>
#include <memory>
#include <comutil.h>
#include <strsafe.h>
#include <activeds.h>
#include <string_view>
#include <string>
#include <iterator>

#pragma warning(disable: 26495)

#include "sqlite3/sqlite3.h"

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)