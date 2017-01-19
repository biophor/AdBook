#pragma warning(push,1)
#include <QtWidgets>
#pragma warning(pop)

#include <Dpapi.h>
#include <Wincrypt.h>

#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include "AdBookBL/export.h"
#pragma warning(pop)
#include <boost/scope_exit.hpp>

#include <boost/numeric/conversion/cast.hpp>
