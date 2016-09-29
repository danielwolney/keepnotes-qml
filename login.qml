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
    property real pixelSize: height * 0.03
    Material.accent: "#FBC02D"
    Material.background: "#FFF176"

    Connections {
        target: app
        onLoginSuccessful: {
            loading.close();
            loginWindow.close();
        }
        onLoginError: {
            loading.close();
            print(errorMessage);
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
                inputMethodHints: Qt.ImhPreferLowercase
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
                font.pixelSize: pixelSize
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
//            Label {
//                font.pixelSize: pixelSize
//                Layout.fillHeight: true
//                text: "OU"
//                anchors.horizontalCenter: parent.horizontalCenter
//            }

//            MaterialButton {
//                font.pixelSize: pixelSize
//                Layout.fillHeight: true
//                anchors {
//                    left: parent.left;
//                    right: parent.right;
//                }
//                text: "Criar uma conta"
//            }
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

}
