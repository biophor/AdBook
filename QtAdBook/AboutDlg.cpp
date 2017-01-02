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