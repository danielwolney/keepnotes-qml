import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import "qrc:/components"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 420
    height: 680
    title: qsTr("Keepnotes")
    property real pixelSize: height * 0.03
//    Material.accent: "#FBC02D"
//    Material.background: "#FFF176"
}
