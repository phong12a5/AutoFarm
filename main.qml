import QtQuick 2.9
import QtQuick.Window 2.2
import AppEnums 1.0
import QtQuick.Controls 2.0

Window {
    id: rootScreen
//    width: 500
//    height: 800
    visible: true


    Text {
        text: qsTr("Token")
        anchors.horizontalCenter: tokenField.horizontalCenter
        anchors.bottom: tokenField.top
        anchors.bottomMargin: 50
    }
    TextField{
        id: tokenField
        anchors.centerIn: parent
        anchors.right: parent.right
        width: contentWidth + 30
        height: font.pixelSize + 20
        text: APP_MODEL.token
    }

    Rectangle{
        id: startBtn
        width: 300
        height: 150
        radius: 4
        color: "grey"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        opacity: clickArea.pressed? 1 : 0.8

        Text {
            anchors.centerIn: parent
            text: qsTr("Start")
        }
        MouseArea{
            id: clickArea
            anchors.fill: parent
            onClicked: {
                // do something
                APP_MODEL.token = tokenField.text
                APP_MODEL.start()
            }
        }
    }
}
