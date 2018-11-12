import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page_view_manual
    width: 600
    height: 400

    Column {
        id: view_manual_main_layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 25
        spacing: 10

        Rectangle{id: gap; width: 10; height: 8;}

        /* Sensors */
        Row {

            id: column_buttons
            anchors.top: gap.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25
            spacing: 5

            RadioButton {
                id: radioButton_Horizontal
                width: 110
                height: 20
                text: qsTr("Horizontal")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }

            RadioButton {
                id: radioButton_Vertical
                width: 90
                height: 20
                text: qsTr("Vertical")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }

            RadioButton {
                id: radioButton_Laser
                width: 80
                height: 20
                text: qsTr("Laser")
                font.pointSize: 11
                display: AbstractButton.TextBesideIcon
                checkable: false
                autoExclusive: false
            }
        }

        /* Manual control valves & cutters */
        Row {
            id: manual_controls
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: column_buttons.bottom
            anchors.topMargin: 20
            spacing: 10

            Switch {
                id: switch_Valvs
                text: qsTr("VALVS")
            }

            Switch {
                id: switch_Cutter
                text: qsTr("CUTTER")
            }
        }

        Row
        {
            id: joystick_button
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            anchors.topMargin: 80
            anchors.top: manual_controls.bottom
            /* Manual control motors */
            VirtualJoystick
            {
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }
}

