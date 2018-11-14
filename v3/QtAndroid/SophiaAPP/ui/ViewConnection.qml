import QtQuick 2.9
import QtQuick.Controls 2.2
import QmlMainClass 1.0

Page {
    width: 600
    height: 400

    QmlMainClass
    {
        id: cpp
    }

    Column
    {
        id: main_column
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Rectangle{id: gap; width: 10; height: 5;}


        Column
        {
            id: instructions_column
            width: 300
            Text {
                id: howto
                text: qsTr("Connection info:")
                font.bold: true
            }

            TextArea {
                width: 300
                height: 160
                text: qsTr("1. Connect to RPi hotspot:
        o AP Name: SophiaController
        o Passwd : soPhiA2018!
 2. Connect to RPi:
        o IP Address: 192.168.1.1
        o Port  1337
 3. Click CONNECT then wait for result! ")
                wrapMode: Text.WordWrap
            }
        }

        Row
        {
            id: connection_status
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Label
            {
                text: "Status:"
                font.bold: true
            }

            Label
            {
                id: status_label
                text: "NOT connected"
            }
        }

        Button
        {
            id: button_connect
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Connect"

            onPressed: {
                 cpp.onButtonPressed_Connect("127.0.0.1", 1337);
            }

            onReleased: {
            }
        }

    }
}
