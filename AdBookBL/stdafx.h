// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
//
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxole.h>         // MFC OLE classes
//#include <afxodlgs.h>       // MFC OLE dialog classes
//#include <afxdisp.h>        // MFC Automation classes
//#endif // _AFX_NO_OLE_SUPPORT
//
//#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>                      // MFC ODBC database classes
//#endif // _AFX_NO_DB_SUPPORT
//
//#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>                     // MFC DAO database classes
//#endif // _AFX_NO_DAO_SUPPORT
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
//#endif
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>                     // MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxmt.h>
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
#include <comutil.h>
#include <strsafe.h>
#include <activeds.h>
#include <eskimo/export.h>
#include <boost/assert.hpp>
#include <boost/scope_exit.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string/replace.hpp>
//#include <boost/interprocess/sync/scoped_lock.hpp>
//#include <boost/interprocess/sync/interprocess_mutex.hpp>
//#include <boost/interprocess/sync/named_mutex.hpp>


#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

