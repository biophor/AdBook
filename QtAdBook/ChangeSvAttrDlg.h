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
#ifndef CHANGESVATTRDLG_H
#define CHANGESVATTRDLG_H

#include "ui_ChangeSvAttrDlg.h"

class ChangeSvAttrDlg : public QDialog
{
    Q_OBJECT

public:
    ChangeSvAttrDlg(
        const QString & objectDn,
        const QString & currentValue,
        const adbook::Attributes::AttrId attrToChange,
        QWidget *parent = 0
    );
    ~ChangeSvAttrDlg();
    QString GetNewValue() const;
private slots:
    void OnOk();
    void OnCancel();
private:
    QString _newValue;
    const QString _currentValue;
    const adbook::Attributes::AttrId _attrToChange;
    Ui::ChangeSvAttrDlg ui;
};

#endif // CHANGESVATTRDLG_H
