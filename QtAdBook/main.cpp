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
