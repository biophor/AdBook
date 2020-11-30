import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Button {
    id: btn
    implicitWidth: 100
    implicitHeight: 25
    text: "Text"
    style: ButtonStyle {
        label: Label {
            width: 100
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment : Text.AlignVCenter
            anchors.fill: parent
            text: btn.text
            font.pointSize: 10
        }
    }
}
