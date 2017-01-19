// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "AboutDlg.h"

AboutDlg::AboutDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedHeight(size().height());
    connect(ui.pushButton, &QPushButton::clicked, this, &AboutDlg::OnOk);
}

AboutDlg::~AboutDlg()
{

}

void AboutDlg::OnOk()
{
    close();
}