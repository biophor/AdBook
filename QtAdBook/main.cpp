// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "QtAdBook.h"
#include "ConnectionSettings.h"
#include "Application.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    Application a(argc, argv);        
    QtAdBook w;
    w.show();
    return a.exec();
}
