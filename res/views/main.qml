import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import "qrc:/components"
import Models 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    minimumHeight: 680
    minimumWidth: 420
    title: qsTr("Keepnotes")
    Material.primary: "#FBC02D"
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: initialPage
        Loader {
            id: initialPage
            source: "qrc:/views/listnotes.qml"
        }
    }
}
