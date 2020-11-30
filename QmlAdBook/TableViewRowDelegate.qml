import QtQuick 2.3

Rectangle {
    height: 20
    SystemPalette {
        id: myPalette;
        colorGroup: SystemPalette.Active
    }
    color: {
        var baseColor = styleData.alternate ? myPalette.alternateBase: myPalette.base
        return styleData.selected ? myPalette.highlight: baseColor
    }
}
