#pragma once

#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4458)  // for gdi+
#include <windows.h>
#include <gdiplus.h>
#include <objidl.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

//#define NOMINMAX
#include <memory>
#include <future>
#include <mutex>
#include <map>
#include <any>
#include <variant>
#include <QObject>
#include <QGuiApplication>
#include <QAbstractListModel>
#include <QSettings>
#include <QString>
#include <QTime>
#include <QRect>
#include <QVector>
#include <QList>
#include <QBuffer>
#include <QDataStream>
#include <QPainter>
#include <QClipboard>
#include <QFileInfo>
#include <strsafe.h>

#include "AdBookBL/export.h"
#pragma warning(pop)

