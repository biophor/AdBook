/*
Copyright (C) 2015-2017 Goncharov Andrei.

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
#include "SettingsDlg.h"
#include "WaitCursor.h"
#include "ConnectionSettings.h"

SettingsDlg::SettingsDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);    
    SetupSignals();
    SetupWindowProperties();
    ReadSettings();
    LoadWindowSettings();
}

SettingsDlg::~SettingsDlg()
{
    
}

void SettingsDlg::SetupSignals()
{
    bool b = connect(ui.ok, SIGNAL(clicked(bool)), SLOT(OnOk()));
    Q_ASSERT(b);
    b = connect(ui.cancel, SIGNAL(clicked(bool)), SLOT(OnCancel()));
    Q_ASSERT(b);
    b = connect(ui.test, SIGNAL(clicked(bool)), SLOT(OnTestClicked()));
    Q_ASSERT(b);
    b = connect(ui.defaultDc, SIGNAL(stateChanged(int)), SLOT(OnDefaultDcStateChanged(int)));
    Q_ASSERT(b);
    b = connect(ui.currentUser, SIGNAL(stateChanged(int)), SLOT(OnCurrentUserStateChanged(int)));
    Q_ASSERT(b);
    b = connect(ui.displayPassword, SIGNAL(stateChanged(int)), SLOT(OnDisplayPasswordStateChanged(int)));
    Q_ASSERT(b);
}

void SettingsDlg::SetupWindowProperties()
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowSystemMenuHint);
    setFixedHeight(size().height());

    QPalette palette;
    palette.setColor(QPalette::Disabled, QPalette::WindowText,
        QApplication::palette().color(QPalette::Disabled, QPalette::WindowText));
    ui.groupCred->setPalette(palette);
}

void SettingsDlg::OnOk()
{
    if (!ui.defaultDc->isChecked() && ui.domain->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QApplication::applicationName(), 
            tr("You should specify a domain or a domain controller."));
        ui.domain->setFocus();
        return;
    }
    if (!ui.currentUser->isChecked() && ui.login->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("You should specify a username."));
        ui.login->setFocus();
        return;
    }
    if (!ui.currentUser->isChecked() && ui.password->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("You should specify a password."));
        ui.password->setFocus();
        return;
    }
    WriteSettings();    
    done(QDialog::Accepted);
}

void SettingsDlg::OnCancel()
{
    done(QDialog::Rejected);
}

void SettingsDlg::OnDefaultDcStateChanged(int state)
{
    ui.labelSpecDC->setEnabled(Qt::Checked != state);
    ui.domain->setEnabled(Qt::Checked != state);
}

void SettingsDlg::OnCurrentUserStateChanged(int state)
{
    ui.groupCred->setEnabled(Qt::Checked != state);

    // Create a palette
    
}

void SettingsDlg::OnTestClicked()
{    
    try
    {
        adbook::ConnectionParams connectionParams;
        connectionParams.SetDC(ui.domain->text().trimmed().toStdWString());
        connectionParams.SetLogin(ui.login->text().trimmed().toStdWString());
        connectionParams.SetPassword(ui.password->text().trimmed().toStdWString());
        connectionParams.CurrentDomain(ui.defaultDc->isChecked());
        connectionParams.CurrentUserCredentials(ui.currentUser->isChecked());
        adbook::AdConnector ac(connectionParams);
        WaitCursor wc;
        ac.Connect();
        wc.RestoreCursor();
        QMessageBox::information(this, QApplication::applicationName(), 
            tr("Connection succeeded."), QMessageBox::Ok);
    }
    catch (const adbook::HrError & e)
    {
        QString error = tr("Failed to connect. ") + QString::fromStdWString(e.What());
        QMessageBox::warning(this, QApplication::applicationName(), error, QMessageBox::Ok);
    }
    catch (const std::exception & e)
    {
        QMessageBox::warning(this, QApplication::applicationName(), e.what(), QMessageBox::Ok);        
    }
}

namespace SettingsWndSettings
{
const QString wndPos = "SettingsWindowPosition";

}

void SettingsDlg::SaveWindowSettings()
{
    QSettings appSettings(QApplication::organizationName(), QApplication::applicationName());
    appSettings.setValue(SettingsWndSettings::wndPos, geometry());
}

void SettingsDlg::LoadWindowSettings()
{
    QSettings appSettings(QApplication::organizationName(), QApplication::applicationName());
    if (appSettings.contains(SettingsWndSettings::wndPos)) {
        setGeometry(appSettings.value(SettingsWndSettings::wndPos, geometry()).toRect());
    }
}

void SettingsDlg::ReadSettings()
{
    auto & connectionSettings = ConnectionSettings::Instance();
    ui.domain->setText(QString::fromStdWString(connectionSettings.GetDC()));
    ui.login->setText(QString::fromStdWString(connectionSettings.GetLogin()));
    ui.defaultDc->setChecked(connectionSettings.CurrentDomain());
    ui.currentUser->setChecked(connectionSettings.CurrentUserCredentials());
    ui.password->setText(QString::fromStdWString(connectionSettings.GetPassword()));
    ui.forgetPassword->setChecked(connectionSettings.ForgetPassword());    
}

void SettingsDlg::WriteSettings()
{
    auto & connectionSettings = ConnectionSettings::Instance();
    connectionSettings.SetDC(ui.domain->text().trimmed().toStdWString());
    connectionSettings.SetLogin(ui.login->text().trimmed().toStdWString());
    connectionSettings.SetPassword(ui.password->text().trimmed().toStdWString());
    connectionSettings.CurrentDomain(ui.defaultDc->isChecked());
    connectionSettings.CurrentUserCredentials(ui.currentUser->isChecked());
    connectionSettings.ForgetPassword(ui.forgetPassword->isChecked());
    connectionSettings.Save();    
}

void SettingsDlg::OnDisplayPasswordStateChanged(int state)
{
    if (Qt::Checked == state) {
        ui.password->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui.password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    }
}

void SettingsDlg::closeEvent(QCloseEvent *)
{
    SaveWindowSettings();    
}