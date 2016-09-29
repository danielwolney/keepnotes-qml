import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import "qrc:/components"
import Models 1.0

Page {
    id: root
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
            text: "Nova nota"
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
                app.notes.addNote(textArea.text);
                stackView.pop();
            }
        }
    }
    TextArea {
        id: textArea
        anchors.fill: parent
        background: Item {}
        focus: true
    }


}
