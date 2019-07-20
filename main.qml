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

    Flickable{
        anchors.fill: parent
        contentWidth: log.width
        contentHeight: log.height

        Rectangle{
            clip: true
            color: "green"
            opacity: 0.2
            visible:  APP_MODEL.deviceType == "Nox Device"
            anchors.fill: parent
        }

        Text {
            id: log
            width: parent.width
            height: contentHeight
            wrapMode: Text.WordWrap
            anchors.bottom: parent.bottom
            text: APP_MODEL.logContent
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
        interval: 15000
        repeat: false
        onTriggered: {
            if(startBtn.enabled){
                startBtn.enabled = false
                APP_MODEL.token = tokenField.text
                APP_MODEL.start()
            }
        }
    }

    Image {
        id: testImage
        source: APP_MODEL.testingImageSource == ""? "" : "file:///" + APP_MODEL.testingImageSource
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        visible: APP_MODEL.testImageMode

        Rectangle{
            width: parent.width
            height: textF.contentHeight
            anchors.fill: textF
            opacity: 0.7
        }

        Text{
            id: textF
            text: APP_MODEL.screenName
            font.pixelSize: 15
            color: "red"
            anchors.centerIn: parent
        }
    }

    Component.onCompleted: {
        autoStartTimer.start()
    }
}
