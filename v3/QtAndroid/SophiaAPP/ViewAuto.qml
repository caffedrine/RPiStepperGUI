import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    width: 600
    height: 400

    RowLayout {
        width: 300
        anchors.horizontalCenter: parent.horizontalCenter
        TextField {
            id: textField
            width: 190

            text: qsTr("0000")
            anchors.fill: parent

            font.pointSize: 50
            maximumLength: 4
            inputMethodHints: Qt.ImhDigitsOnly
        }

        Text {
            id: units
            text: qsTr("mm")
            font.pointSize: 30
        }
    }

    Column {

        anchors.centerIn: parent
        anchors.verticalCenterOffset: 10

        Row {
            Button {
                id: button_auto_reset
                width: 400
                height: 100

                text: qsTr("Reset")
            }
        }

        Row {

            width: 250
            anchors.topMargin: 10
            anchors.verticalCenterOffset: 10

            Button {
                anchors.verticalCenterOffset: 10
                id: button_auto_stop
                width: 200
                height: 100
                text: qsTr("Stop")

                //onClicked: signal test_sig()
            }

            Button {
                anchors.verticalCenterOffset: 10
                id: button_auto_start
                width: 200
                height: 100
                text: qsTr("Start")
            }
        }
    }
}
