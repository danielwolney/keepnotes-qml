import QtQuick 2.7
import QtQuick.Controls 2.0
import "qrc:/components"

Button {
    id: root
    property alias color: back.fillColor
    property alias radius: back.radius
    property alias wrapMode: label.wrapMode
    property alias textColor: label.color
    property alias elevation: back.elevation
    background: BaseComponent {
        id: back
        elevation: 1
        property color fillColor: '#FBC02D'
        anchors.fill: parent
        color: parent.enabled ? (parent.pressed ? Qt.darker(fillColor,1.2): fillColor): "lightGrey"
        radius: 2
    }
    contentItem: Label {
        id: label
        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignVCenter
        color: 'white'
        text: parent.text.toUpperCase()
    }
}
