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
            Label {
                id: emptyMsg
                anchors.centerIn: parent;
                visible: listNotes.count < 1
                text: "Sua notas aparecerão aqui"
            }

            ListView {
                id: listNotes
                anchors {
                    top: parent.top; topMargin: parent.height * 0.02
                    bottom: parent.bottom; bottomMargin: parent.height * 0.02
                    horizontalCenter: parent.horizontalCenter
                }
                property real maximumWidth: 630
                property real minimumCardHeight: lblTitle.implicitHeight * 4
                property real relativeParentWidth: parent.width * 0.96
                width: (relativeParentWidth < maximumWidth ? relativeParentWidth: maximumWidth);
                model: app.notes
                spacing: parent.height * 0.01
                delegate: Card {
                    property int startX
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
                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        drag.target: parent
                        drag.axis: Drag.XAxis
                        drag.minimumX: 0 - parent.width
                        drag.maximumX: parent.width
                        onClicked: {
                            stackView.push("qrc:/viewnote.qml", {"editMode": true, "itemIndex": index});
                        }
                        onPressed: {
                            mouse.accepted = true;
                            startX = parent.x;
                        }
                        onReleased: {
                            var deltaX = parent.x - startX;
                            if (Math.abs(deltaX) < (parent.width * 0.5)) {
                                parent.x = 0;
                            } else {
                                parent.x = (deltaX > 0 ? drag.maximumX: drag.minimumX);
                            }
                        }
                    }
                    onXChanged: {
                        if (x == mouseArea.drag.maximumX || x == mouseArea.drag.minimumX) {
                            app.notes.removeNote(index);
                        }
                    }
                    Behavior on x { NumberAnimation { duration: 200 } }
                }
                add: Transition {
                    NumberAnimation { properties: "x, y"; duration: 500; easing.type: Easing.InOutExpo }
                    NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 500}
                }
                displaced: Transition {
                    NumberAnimation { properties: "x, y"; duration: 500; easing.type: Easing.InOutExpo }
                }
                ScrollBar.vertical: ScrollBar{}
            }

            MaterialButton {
                id: addButton
                elevation: 4
                anchors {
                    right: parent.right; rightMargin: parent.height * 0.03
                    bottom: parent.bottom;  bottomMargin: parent.height * 0.03
                }
                implicitHeight: emptyMsg.implicitHeight * 4
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
