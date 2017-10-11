#pragma warning(push,1)
#include <QDialog>
#include <QtWidgets>
#include <QtWidgets/QDialog>
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QStyledItemDelegate>
#include <qsettings.h>
#pragma warning(pop)

#include <Dpapi.h>
#include <Wincrypt.h>

#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include "AdBookBL/export.h"
#pragma warning(pop)
#include <variant>
#include <boost/scope_exit.hpp>
#include <boost/numeric/conversion/cast.hpp>
