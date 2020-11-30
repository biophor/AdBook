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

Dialogs.Dialog {
    id: root
    title: qsTr("About...")
    modality: Qt.WindowModal

    Controls.Label {
        onLinkActivated: {
            Qt.openUrlExternally(link)
        }
        anchors.centerIn: parent
        padding: 10
        font.pointSize: 12
        text: qsTr("<h4>Active Directory Contact Book, Version 0.7.1 (BETA)</h4>
                    <br>Copyright(C) 2015-2021 Andrei Goncharov
                    <br>The latest release can be downloaded here:
                    <br><a href=\"https://github.com/biophor/AdBook/releases\">
                            https://github.com/biophor/AdBook/releases</a>")
    }
}


