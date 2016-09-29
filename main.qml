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
        Page {
            id: initialPage
            header: ToolBar {
                ToolButton {
                    id: btnMenu
                    anchors {
                        bottom: parent.bottom
                        top: parent.top
                    }
                    width: height
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/img/menu.svg"
                        fillMode: Image.PreserveAspectFit
                    }
                    onClicked: {
                    }
                }
                Label {
                    id: lblTitle
                    anchors.left: btnMenu.right; anchors.leftMargin: parent.width * 0.01
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                    color: "white"
                    text: "Keepnotes"
                }
            }
            ListView {
                id: listNotes
                anchors {
                    top: parent.top; topMargin: parent.height * 0.02
                    bottom: parent.bottom; bottomMargin: parent.height * 0.02
                    horizontalCenter: parent.horizontalCenter
                }
                verticalLayoutDirection: ListView.BottomToTop

                property real maximumWidth: 630
                property real minimumCardHeight: lblTitle.implicitHeight * 4
                property real relativeParentWidth: parent.width * 0.96
                width: (relativeParentWidth < maximumWidth ? relativeParentWidth: maximumWidth);
                model: app.notes
                spacing: parent.height * 0.01
                delegate: Card {
                    color: "white"
                    width: ListView.view.width
                    height: (listNotes.minimumCardHeight <= label.implicitHeight ?
                                 label.implicitHeight + lblTitle.implicitHeight:
                                 listNotes.minimumCardHeight )
                    Label {
                        id: label
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        wrapMode: Text.WordWrap
                        text: model.text
                        width: parent.width * .9
                        height: parent.height * .9
                    }
                }
            }

            MaterialButton {
                id: addButton
                elevation: 4
                anchors {
                    right: parent.right; rightMargin: parent.height * 0.03
                    bottom: parent.bottom;  bottomMargin: parent.height * 0.03
                }
                implicitHeight: lblTitle.implicitHeight * 4
                width: height
                radius: height/2
                color: "#FF5722"
                Image {
                    source: "qrc:/img/plus.svg"
                    anchors.centerIn: parent
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    sourceSize.width:  parent.height * 0.5
                    sourceSize.height: parent.height * 0.5
                }
                onClicked: {
                    stackView.push("qrc:/viewnote.qml");
                }
            }
        }
    }
}
