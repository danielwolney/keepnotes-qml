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
    property real pixelSize: Math.min(20, Math.max(height, width) * 0.03)
    function askLogout() {
        popupaskLogout.open();
    }
    Popup {
        id: popupaskLogout
        property real maximumWidth: 420
        property real maximumHeight: 190
        property real relativeParentWidth: Math.min(parent.width, parent.height) * 0.65
        property real relativeParentHeight: Math.max(parent.width, parent.height) * 0.25
        x: parent.width / 2 - contentWidth / 2
        y: parent.height / 2 - contentHeight / 2
        contentHeight: elementsColumn.height //Math.min(relativeParentHeight, maximumHeight)
        contentWidth: elementsColumn.width
        modal: true
        focus: true
        property string errorMsg: "Fazer logout?"
        contentItem: ColumnLayout {
            id: elementsColumn
            clip: true
            width: Math.min(popupaskLogout.relativeParentWidth, popupaskLogout.maximumWidth);
            height: msgColumn.implicitHeight + spacing + okButton.implicitHeight
            spacing: 10
            Column {
                id: msgColumn
                width: parent.width
                spacing: 10
                Label {
                    text: "Sair"
                    font.bold: true
                    font.pixelSize: pixelSize
                }
                Label {
                    id: labelMsg

                    width: parent.width
                    text: popupaskLogout.errorMsg
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    font.pixelSize: pixelSize
                }
                Layout.fillWidth: true
            }
            Button {
                id: okButton
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }
                text: "OK"
                flat: true
                font.pixelSize: pixelSize
                background.anchors.bottom: okButton.bottom
                bottomPadding: 0
                Material.foreground: "#FBC02D"
                onClicked: {
                    popupaskLogout.close()
                    app.logout();
                    mainWindow.close();
                }
            }
            Button {
                id: cancelButton
                anchors {
                    right: okButton.left
                    bottom: parent.bottom
                }
                text: "CANCELAR"
                flat: true
                font.pixelSize: pixelSize
                background.anchors.bottom: cancelButton.bottom
                bottomPadding: 0
                Material.foreground: "#FBC02D"
                onClicked: {
                    popupaskLogout.close()
                }
            }
        }
    }
    Drawer {
        id: drawer
        width: Math.min(parent.width, parent.height) / 3 * 2
        height: parent.height
        Column {
            visible: parent.visible
            anchors.fill: parent
            Pane {
                width: parent.width
                height: parent.height * 0.25
                Material.background: Material.primary
                Label {
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 2
                    }
                    color: "white"
                    font.bold: true
                    text: "danielwolney@gmail.com"
                }
            }
            ItemDelegate {
                width: parent.width
                text: "Notas"
                onClicked: {
                    stackView.pop();
                    drawer.close();
                }
            }
            ItemDelegate {
                width: parent.width
                text: "Sair"
                onClicked: {
                    drawer.close();
                    askLogout();
                }
            }
        }
    }
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
