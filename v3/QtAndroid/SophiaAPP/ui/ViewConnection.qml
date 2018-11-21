import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    width: 600
    height: 400

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
                height: 110
                text: qsTr("1. Connect to RPi hotspot:
        o AP Name: SophiaController
        o Passwd : soPhiA2018!
 2. When GREEN led is ON just press CONNECT then wait for result! ")
                wrapMode: Text.WordWrap
            }
        }

        Row
        {
            id: connection_ip
            anchors.topMargin: 10
            spacing: 10

            Label
            {
                text: "IP Address:"
                font.bold: true
            }

            Label
            {
                id: connection_ip_label
                text: root.ip_address
            }
        }

        Row
        {
            id: connection_port
            spacing: 10

            Label
            {
                text: "Port:"
                font.bold: true
            }

            Label
            {
                id: connection_port_label
                text: root.port
            }
        }
        Row
        {
            id: connection_status
            anchors.topMargin: 10
            spacing: 10

            Label
            {
                text: "Status:"
                font.bold: true
            }

            Label
            {
                id: status_label
                text: root.connection_status_label
            }
        }

        Button
        {
            id: button_connect
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Connect"

            onPressed: {
               cpp.connectButtonPressed();
            }

            onReleased:
            {
                cpp.connectButtonReleased();
            }

        }

    }
}
