import QtQuick 2.9
import QtQuick.Window 2.2
import AppEnums 1.0
import QtQuick.Controls 2.0

Window {
    id: rootScreen
    width: 480
    height: 720
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

    Item{
        id: deviceTypeItem
        height: deviceTypeField.height
        width: deviceTypeField.width + info.width + 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: tokenField.bottom
        anchors.topMargin: 50

        Text {
            id: deviceTypeField
            text: qsTr("Device Type: ")
            font.pixelSize: tokenField.font.pixelSize
        }

        Text{
            id: info
            anchors.verticalCenter: deviceTypeField.verticalCenter
            anchors.left: deviceTypeField.right
            anchors.leftMargin: 10
            font.pixelSize: tokenField.font.pixelSize
            text: APP_MODEL.deviceType
        }
    }

    Item{
        width: autoStartTitle.width + autoStartCheckBox.indicator.width + 20
        height: autoStartCheckBox.height
        anchors.top: deviceTypeItem.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        Text{
            id: autoStartTitle
            font.pixelSize: tokenField.font.pixelSize
            anchors.verticalCenter: autoStartCheckBox.verticalCenter
            anchors.left: parent.left
            text: "Auto starting: "
        }

        CheckBox{
            id: autoStartCheckBox
            checkState: APP_MODEL.autoStart? Qt.Checked : Qt.Unchecked
            indicator.width: tokenField.font.pixelSize
            indicator.height: tokenField.font.pixelSize
            anchors.right: parent.right
            onClicked: APP_MODEL.autoStart = !APP_MODEL.autoStart
        }

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
                startBtn.enabled = false
                APP_MODEL.token = tokenField.text
                APP_MODEL.start()
            }
        }
    }

    Timer{
        id: autoStartTimer
        interval: 60000
        repeat: false
        onTriggered: {
            if(startBtn.enabled){
                startBtn.enabled = false
                APP_MODEL.token = tokenField.text
                APP_MODEL.start()
            }
        }
    }

    Component.onCompleted: {
        autoStartTimer.start()
    }
}
