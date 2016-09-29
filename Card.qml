import QtQuick 2.7
import "qrc:/components"

BaseComponent {
    elevation: 1
    radius: fullWidth || fullHeight ? 0 : 2
    elevationInverted: false
    property bool flat: false
    border.color: flat ? Qt.rgba(0,0,0,0.2) : "transparent"
}
