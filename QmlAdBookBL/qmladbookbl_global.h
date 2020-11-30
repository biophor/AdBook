#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QMLADBOOKBL_LIB)
#  define QMLADBOOKBL_EXPORT Q_DECL_EXPORT
# else
#  define QMLADBOOKBL_EXPORT Q_DECL_IMPORT
# endif
#else
# define QMLADBOOKBL_EXPORT
#endif
