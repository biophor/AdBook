import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3 as Dialogs
import QtQuick.Controls 1.4 as Controls
import Qt.labs.settings 1.0
import biophor.local 1.0

Dialogs.Dialog {
    id: settingsDlg
    title: qsTr("Settings")
    modality: Qt.WindowModal

    height: 400
    width: 300
    standardButtons: Dialogs.Dialog.Ok | Dialogs.Dialog.Cancel

    // Settings doesn't work in this case. Don't know why
    /*
    Settings {
        category: "SettingsDlg"
        property alias x: settingsDlg.x
        property alias y: settingsDlg.y
        property alias width: settingsDlg.width
        property alias height: settingsDlg.height
    }
    */

    Component.onCompleted: {
        settingsDlgViewModel.load()
    }

    onAccepted: {
        settingsDlgViewModel.login = loginTextField.text;
        settingsDlgViewModel.password = passwordTextField.text
        settingsDlgViewModel.address = addressTextField.text

        settingsDlgViewModel.save()
    }
    onRejected: {

    }

    SettingsDlgViewModel {
        id: settingsDlgViewModel
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        Controls.GroupBox {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            title: qsTr("Connection:")

            ColumnLayout {
                anchors.fill: parent
                Controls.ExclusiveGroup {
                    id: connectionRadioGroup
                }
                Controls.RadioButton {
                    id: connectCurrentDomainRadioButton
                    text: qsTr("Connect the domain you logged on to")
                    checked: settingsDlgViewModel.connectCurrentDomain
                    exclusiveGroup: connectionRadioGroup
                    Binding {
                        target: settingsDlgViewModel;
                        property: "connectCurrentDomain";
                        value: connectCurrentDomainRadioButton.checked
                        delayed: true
                    }
                }
                Controls.RadioButton {
                    id: connectSpecifiedAddress
                    checked: !settingsDlgViewModel.connectCurrentDomain
                    text: qsTr("Connect the following domain:")
                    exclusiveGroup: connectionRadioGroup
                }
                Controls.TextField {
                    id: addressTextField
                    text: settingsDlgViewModel.address
                    enabled: connectSpecifiedAddress.checked
                    maximumLength: 50
                    Layout.fillWidth: true
                    Layout.leftMargin: 15
                    /*
                    Binding {
                        target: settingsDlgViewModel;
                        property: "domain";
                        value: domainTextField.text
                        delayed: true
                    }
                    */
                }
            }
        }

        Controls.GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("Authentication:")
            ColumnLayout {
                anchors.fill: parent
                Controls.ExclusiveGroup {
                    id: authenticationRadioGroup
                }
                Controls.RadioButton {
                    id: connectAsCurrentUserRadioButton
                    text: qsTr("Use the current user's credentials")
                    checked: settingsDlgViewModel.connectAsCurrentUser
                    exclusiveGroup: authenticationRadioGroup

                    Binding {
                        target: settingsDlgViewModel;
                        property: "connectAsCurrentUser";
                        value: connectAsCurrentUserRadioButton.checked
                        delayed: true
                    }
                }
                Controls.RadioButton {
                    text: qsTr("Connect using the following credentials:")
                    checked: !settingsDlgViewModel.connectAsCurrentUser
                    exclusiveGroup: authenticationRadioGroup
                }
                Controls.Label {
                    enabled: !connectAsCurrentUserRadioButton.checked
                    Layout.leftMargin: 15
                    text: qsTr("Login:")
                }
                Controls.TextField {
                    enabled: !connectAsCurrentUserRadioButton.checked
                    Layout.fillWidth: true
                    Layout.leftMargin: 15
                    maximumLength: 50
                    id: loginTextField
                    text: settingsDlgViewModel.login
                    /*
                    Binding {
                        target: settingsDlgViewModel;
                        property: "login";
                        value: loginTextField.text
                        delayed: true
                    }
                    */
                }
                Controls.Label{
                    text: qsTr("Password:")
                    enabled: !connectAsCurrentUserRadioButton.checked
                    Layout.leftMargin: 15
                }
                Controls.TextField {
                    id: passwordTextField
                    echoMode: TextInput.Password
                    enabled: !connectAsCurrentUserRadioButton.checked
                    text: settingsDlgViewModel.password
                    maximumLength: 50
                    Layout.fillWidth: true
                    Layout.leftMargin: 15
                }
                Controls.CheckBox {
                    text: qsTr("Display the password")
                    onCheckedChanged: {
                        if (checked === true) {
                            passwordTextField.echoMode = TextInput.Normal
                        }
                        else{
                            passwordTextField.echoMode = TextInput.Password
                        }
                    }
                }
                Controls.Button {
                    id: testSettingsButton
                    action: testSettingsAction
                }
                Item {
                    Layout.fillHeight: true
                }
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }

    Controls.Action {
        id: testSettingsAction
        text: "&Test"
        enabled: {
            var loginSpecified = loginTextField.text.trim().length > 0
            var passwordSpecified = passwordTextField.text.length > 0
            var addressSpecified = addressTextField.text.trim().length > 0
            if (!connectCurrentDomainRadioButton.checked && !addressSpecified){
                return false;
            }
            if (!connectAsCurrentUserRadioButton.checked && (!loginSpecified || !passwordSpecified)){
                return false;
            }
            return true;
        }
        onTriggered: {
            settingsDlgViewModel.login = loginTextField.text;
            settingsDlgViewModel.password = passwordTextField.text
            settingsDlgViewModel.address = addressTextField.text

            if (settingsDlgViewModel.testSettings()) {
                messageDialog.text = qsTr("Connection successful")
                messageDialog.informativeText = settingsDlgViewModel.testSettingsDetails
                messageDialog.icon = Dialogs.StandardIcon.Information
            }
            else {
                messageDialog.text = qsTr("Failed to connect")
                messageDialog.informativeText = settingsDlgViewModel.testSettingsDetails
                messageDialog.icon = Dialogs.StandardIcon.Warning
            }
            messageDialog.open()
        }
    }

    Dialogs.MessageDialog {
        id: messageDialog
        title: qsTr("Active Directory Contact Book")
        modality: Qt.WindowModal
        standardButtons: Dialogs.StandardButton.Ok
    }
    /*
    Dialogs.Dialog {
        id: myMessageBox
        title: qsTr("Active Directory Contact Book")
        modality: Qt.WindowModal
        standardButtons: Dialogs.StandardButton.Ok
        property alias text: myMessageBoxText.text

        ColumnLayout {
            anchors.fill: parent
            Controls.Label {
                padding: 10
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                id: myMessageBoxText
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }
    }
    */


}
