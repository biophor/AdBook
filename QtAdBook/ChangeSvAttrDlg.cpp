// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Andrei Goncharov.

This file is part of the 'Active Directory Contact Book'.
'Active Directory Contact Book' is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

'Active Directory Contact Book' is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
'Active Directory Contact Book'. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "ChangeSvAttrDlg.h"

ChangeSvAttrDlg::ChangeSvAttrDlg(
    const QString & objectDn,
    const QString & currentValue,
    const adbook::Attributes::AttrId attrToChange,
    QWidget * parent
)
    : QDialog(parent), _attrToChange(attrToChange), _currentValue(currentValue)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedHeight(size().height());

    QColor mainWindowBgColor = palette().color(QPalette::Window);
    QPalette readOnlyPalette = ui.objectPath->palette();
    readOnlyPalette.setColor(QPalette::Base, mainWindowBgColor);
    ui.objectPath->setPalette(readOnlyPalette);
    ui.attrNames->setPalette(readOnlyPalette);
    ui.currentValue->setPalette(readOnlyPalette);

    ui.currentValue->setText(_currentValue);
    ui.newValue->setText(_currentValue);
    ui.objectPath->setText(objectDn);

    connect(ui.OK, &QPushButton::clicked, this, &ChangeSvAttrDlg::OnOk);
    connect(ui.Cancel, &QPushButton::clicked, this, &ChangeSvAttrDlg::OnCancel);
    auto & attributes = adbook::Attributes::GetInstance();
    const QString uiAttrName = QString::fromStdWString(attributes.GetUiAttrName(attrToChange));
    const QString ldapAttrName = QString::fromStdWString(attributes.GetLdapAttrName(attrToChange));
    const QString attrOid = QString::fromStdWString(attributes.GetAttrOid(attrToChange));
    ui.attrNames->setText(uiAttrName + " / " + ldapAttrName + " / " + attrOid);
}

ChangeSvAttrDlg::~ChangeSvAttrDlg()
{

}

QString ChangeSvAttrDlg::GetNewValue() const
{
    return _newValue;
}

void ChangeSvAttrDlg::OnOk()
{
    if (adbook::Attributes::CommonName == _attrToChange && _newValue.isEmpty()) {
        QMessageBox::warning(this, QApplication::applicationName(), tr("CN cannot be empty."));
        return;
    }
    _newValue = ui.newValue->text().trimmed();
    accept();
}

void ChangeSvAttrDlg::OnCancel()
{
    reject();
}

