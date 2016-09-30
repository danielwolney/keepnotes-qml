import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import "qrc:/components"

ApplicationWindow {
    id: loginWindow
    visible: true
    minimumHeight: 680
    minimumWidth: 420
    title: qsTr("Keepnotes")
    property real pixelSize: Math.min(20, Math.max(height, width) * 0.03)

    Material.accent: "#FBC02D"
    Material.background: "#FFF176"

    Connections {
        target: app
        onLoginSuccessful: {
            loading.close();
            loginWindow.close();
        }
        onLoginError: {
            errorPopup.errorMsg = errorMessage;
            loading.close();
            errorPopup.open();
        }
    }

    Card {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        width: parent.width * .8
        height: parent.height * .7
        ColumnLayout {
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            width: parent.width * .8
            height: parent.height * .8
            spacing: parent.height * 0.02
            Image {
                Layout.fillHeight: true
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
                source: "qrc:/img/logo.png"
                width: parent.width *.9
                height: parent.height *.25

                sourceSize.width: parent.width *.9
                sourceSize.height: parent.height *.25
            }

            TextField {
                id: txtEmail
                Layout.fillHeight: true
                anchors {
                    left: parent.left; leftMargin: parent.width * 0.03
                    right: parent.right; rightMargin: parent.width * 0.03
                }
                placeholderText: "Email"
                font.pixelSize: pixelSize
                inputMethodHints: Qt.ImhEmailCharactersOnly
                text: "danielwolney@gmail.com"
                focus: true;
            }

            TextField {
                id: txtPass
                Layout.fillHeight: true
                anchors {
                    left: parent.left; leftMargin: parent.width * 0.03
                    right: parent.right; rightMargin: parent.width * 0.03
                }
                placeholderText: "Senha"
                font.pixelSize: pixelSize * .9
                echoMode: TextInput.Password
                text: "admin123"
            }
            MaterialButton {
                Layout.fillHeight: true
                anchors {
                    left: parent.left;
                    right: parent.right;
                }
                text: "Entrar"
                font.pixelSize: pixelSize
                onClicked: {
                    loading.open();
                    app.login(txtEmail.text, txtPass.text);
                }
            }
        }
    }

    Popup {
        id: loading
        visible: false
        modal: true
        width: parent.width
        height: parent.height
        x: parent.width / 2 - contentWidth / 2
        y: parent.height / 2 - contentHeight / 2
        background: Item{}
        closePolicy: Popup.NoAutoClose
        onAboutToHide: {
            busy.running = false;
        }
        onAboutToShow: {
            busy.running = true;
        }
        contentItem: BusyIndicator {
            id: busy
            anchors.centerIn: parent
            running: false
        }
    }

    Popup {
        id: errorPopup
        property real maximumWidth: 420
        property real maximumHeight: 190
        property real relativeParentWidth: Math.min(parent.width, parent.height) * 0.65
        property real relativeParentHeight: Math.max(parent.width, parent.height) * 0.25
        x: parent.width / 2 - contentWidth / 2
        y: parent.height / 2 - contentHeight / 2
        contentHeight: Math.min(relativeParentHeight, maximumHeight);
        contentWidth: Math.min(relativeParentWidth, maximumWidth);
        modal: true
        focus: true
        property string errorMsg
        contentItem: ColumnLayout {
            id: elementsColumn
            spacing: 20

            Label {
                text: "Erro"
                font.bold: true
                font.pixelSize: pixelSize
            }

            Label {
                text: errorPopup.errorMsg
                font.pixelSize: pixelSize
            }

            MaterialButton {
                id: okButton
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }
                implicitWidth: contentItem.implicitWidth * 3
                text: "Ok"
                font.pixelSize: pixelSize

                onClicked: {
                    errorPopup.close()
                }
                textColor: Material.primary
                color: "white"
                elevation: 0
            }

        }
    }


}
