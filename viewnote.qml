import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import "qrc:/components"
import Models 1.0

Page {
    id: root
    property bool editMode: false
    property int itemIndex: -1
    Component.onCompleted: {
        textArea.forceActiveFocus();
    }

    header: ToolBar {
        ToolButton {
            id: btnBack
            anchors {
                bottom: parent.bottom
                top: parent.top
            }
            width: height
            Image {
                anchors.centerIn: parent
                source: "qrc:/img/back.svg"
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {
                stackView.pop();
            }
        }
        Label {
            anchors.left: btnBack.right;
            anchors.leftMargin: parent.width * 0.01
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            color: "white"
            text: (editMode ? "Editar nota" : "Nova nota")
        }
        ToolButton {
            id: btnOk
            anchors {
                bottom: parent.bottom
                right: parent.right
                top: parent.top
            }
            width: height
            Image {
                anchors.centerIn: parent
                source: "qrc:/img/check.svg"
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {
                if (editMode) {
                    app.notes.updateNote(itemIndex, textArea.text);
                } else {
                    app.notes.addNote(textArea.text);
                }

                stackView.pop();
            }
        }
    }

    Flickable {
        id: flickable
        contentHeight: textArea.height
        anchors {
            top: parent.top; topMargin: 10
            right: parent.right; rightMargin: 10
            left: parent.left; leftMargin: 10
            bottom: parent.bottom; bottomMargin: 10
        }
        TextArea.flickable: TextArea {
            id: textArea
            wrapMode: TextArea.Wrap
            background: Item {}
            focus: true
            text: (editMode ? app.notes.text(itemIndex) : "")
        }
        ScrollBar.vertical: ScrollBar { }
    }

    Label {
        visible: editMode
        text: Qt.formatDateTime(app.notes.dateTime(itemIndex), "Ultima alteração dd/MM hh:mm");
    }


}
