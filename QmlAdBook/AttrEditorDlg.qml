/*
Copyright (C) 2015-2021 Andrei Goncharov.

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

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3 as Dialogs
import QtQuick.Controls 1.4 as Controls
import biophor.local 1.0

Dialogs.Dialog {
    id: root
    title: qsTr("Attribute Editor")
    modality: Qt.WindowModal
    standardButtons : Dialogs.StandardButton.Ok | Dialogs.StandardButton.Cancel
    width: 300
    property var contactDetailsModel
    property var contactDetailsModelRow: 0

    function load(){
        viewModel.load(contactDetailsModel, contactDetailsModelRow)
    }

    AttrEditorViewModel{
        id: viewModel
    }

    onAccepted:{
        viewModel.newValue = newValue.text.trim()
    }

    ColumnLayout {
        anchors.fill: parent

        Controls.Label {
            text: qsTr("Current value:")
        }
        Controls.TextField {
            id: currentValue
            enabled: true
            readOnly: true
            text: viewModel.currentValue;
            maximumLength: viewModel.maxLength
            Layout.fillWidth: true
        }
        Controls.Label {
            text: qsTr("New value:")
            Layout.topMargin: 10
        }
        Controls.TextField {
            id: newValue
            enabled: true
            text: viewModel.newValue;
            readOnly: false
            maximumLength: viewModel.maxLength
            Layout.fillWidth: true
        }
        Controls.Label {
            text: qsTr("Contact's DN:")
            Layout.topMargin: 10
        }
        Controls.TextField {
            id: dn
            readOnly: true
            text: viewModel.ownerDn
            Layout.fillWidth: true
        }
        Controls.Label {
            text: qsTr("Attribute's details:")
            Layout.topMargin: 10
        }
        Controls.TextArea {
            id: attrInfo
            readOnly: true
            text: viewModel.attrDescription
            Layout.fillWidth: true
            Layout.maximumHeight: 100
        }
        Item {
            Layout.fillHeight: true
        }
    }
}


