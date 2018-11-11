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
        spacing: 15

        Rectangle{id: gap; width: 10; height: 5;}


        Column
        {
            id: instructions_column
            width: 300
            Text {
                id: howto
                text: qsTr("How to connect:")
                font.bold: true
            }

            TextArea {
                width: 300
                height: 160
                text: qsTr("1. Create an AP Hotspot with the following:
    o AP Name: SophiaController
    o Passwd : soPhiA2018!
 2. Power on machinery then wait for the green light
 3. Click CONNECT then wait for result! ")
                wrapMode: Text.WordWrap
            }

        }


        Row
        {
            id: connection_status
            anchors.top: instructions.bottom
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

        }

    }
}
