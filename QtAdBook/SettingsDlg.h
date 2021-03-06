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
#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H


#include "ui_SettingsDlg.h"
#include "AppSettings.h"

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    SettingsDlg(
        std::shared_ptr<adbook::AbstractAdAccessFactory> adFactory,
        AppSettings & appSettings,
        QWidget *parent = 0
    );
    ~SettingsDlg();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void OnDefaultDcStateChanged(int state);
    void OnCurrentUserStateChanged(int state);
    void OnTestClicked();
    void OnOk();
    void OnCancel();
    void OnDisplayPasswordStateChanged(int state);
private:
    void SaveWindowSettings();
    void LoadWindowSettings();

    void ReadSettings();
    void WriteSettings();
    void SetupSignals();
    void SetupWindowProperties();
private:
    std::shared_ptr<adbook::AbstractAdAccessFactory> _adFactory;
    AppSettings & _appSettings;
    Ui::SettingsDlg ui;
};

#endif // SETTINGSDLG_H
