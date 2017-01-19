// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "application.h"


Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    setApplicationName(QObject::tr("Active Directory Contact Book"));
    setOrganizationName(QObject::tr("biophor"));
    setOrganizationDomain("github.com/biophor");
    try {
        ConnectionSettings::Instance().Load();
    }
    catch (const adbook::HrError & e) {
        QMessageBox::warning(nullptr, QApplication::applicationName(),
            QObject::tr("Failed to load settings. ") + 
            QString::fromStdWString(e.What()), 
            QMessageBox::Ok
        );
    }
}

