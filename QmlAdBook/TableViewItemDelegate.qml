import QtQuick 2.12
import QtQuick.Controls 1.4 as QC1
import QtQuick.Controls 2.12 as QC2

Item {
    id: tableViewItemDelegate

    HoverHandler {
        id: hoverHandler
        enabled: true
    }

    QC1.Label {
        id: wtx
        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        color: styleData.textColor
        elide: styleData.elideMode
        font.pointSize: 10
        text: styleData.value
        clip: true

        QC2.ToolTip {
            delay: 250
            parent: wtx
            visible: hoverHandler.hovered && wtx.truncated && text.length > 0
            text: parent.text
            font.pointSize: 10
            font.preferShaping: false
        }
    }
}
