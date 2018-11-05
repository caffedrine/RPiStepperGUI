import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    width: 600
    height: 400

    GridLayout {
        rows: 2
        columns: 1
        anchors.centerIn: parent

        /* Sensors */
        GridLayout {
            rows: 1
            columns: 3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            RadioButton {
                id: radioButton_Horizontal
                x: 53
                y: 85
                width: 126
                height: 20
                text: qsTr("Horizontal")
                checkable: false
                autoExclusive: false
            }

            RadioButton {
                id: radioButton_Vertical
                x: 164
                y: 85
                width: 126
                height: 20
                text: qsTr("Vertical")
                checkable: false
                autoExclusive: false
            }

            RadioButton {
                id: radioButton_Laser
                x: 260
                y: 85
                width: 126
                height: 20
                text: qsTr("Laser")
                display: AbstractButton.TextBesideIcon
                checkable: false
                autoExclusive: false
            }
        }

        /* Manual control valves & cutters */
        GridLayout {
            rows: 1
            columns: 2
            anchors.horizontalCenter: parent.horizontalCenter

            Switch {
                id: switch_Valvs
                clip: true

                text: qsTr("VALVS")
            }

            Switch {
                id: switch_Cutter
                clip: true
                text: qsTr("CUTTER")
            }
        }

        GridLayout
        {
            rows: 1
            columns: 1
            Layout.alignment: parent.horizontalCenter

            /* Manual control motors */
            VirtualJoystick
            {
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }
}
