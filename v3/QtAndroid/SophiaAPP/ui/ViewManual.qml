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

        Rectangle{id: gap1; width: 10; height: 20;}

        Rectangle
        {
            id: sensors_rectangle
            border.color: "black"
            border.width: 2
            width: 310
            height: 150
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25
            anchors.top: gap.bottom

        /* Sensors */
        Row {
            id: column_sensors_v
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25

            RadioButton {
                id: radioButton_VerticalLeft
                width: 155
                height: 20
                text: qsTr("Vertical MASTER")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }

            RadioButton {
                id: radioButton_VerticalRight
                width: 155
                height: 20
                text: qsTr("Vertical SLAVE")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }

        }

        Row {
            id: column_sensors_h
            anchors.top: column_sensors_v.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25

            RadioButton {
                id: radioButton_VerticalMaster
                width: 155
                height: 20
                text: qsTr("Horizontal left")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }

            RadioButton {
                id: radioButton_VerticalSlave
                width: 155
                height: 20
                text: qsTr("Horizontal right")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }

        }

        Row {
            id: column_sensor_laser
            anchors.top: column_sensors_h.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25

            RadioButton {
                id: radioButton_Laser
                width: 150
                height: 20
                text: qsTr("Cutter laser")
                font.pointSize: 11
                checkable: false
                autoExclusive: false
            }
        }

        }

        /* Manual control valves & cutters */
        Row {
            id: column_manuals
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: sensors_rectangle.bottom
            anchors.topMargin: 20
            spacing: 10

            Switch {
                id: switch_Valvs
                text: qsTr("VALVS")

                onClicked:
                {
                    cpp.onSwitchChanged_Valves(switch_Valvs.checked);
                }
            }

            Switch {
                id: switch_Cutter
                text: qsTr("CUTTER")

                onClicked:
                {
                    cpp.onSwitchChanged_Cutter(switch_Cutter.checked);
                }
            }
        }


                Button{
                    id: button_up
                    anchors.top: column_manuals.bottom
                    anchors.topMargin: 15
                    width: 80; height: 40
                    text: "UP"
                    anchors.horizontalCenter: parent.horizontalCenter

                    onPressed: {
                        cpp.onButtonPressed_Up();
                    }

                    onReleased: {
                        cpp.onButtonReleased_Up();
                    }
                }

            Row
            {
                id: row_middle
                anchors.top: button_up.bottom
                anchors.topMargin: 4
                anchors.horizontalCenter: parent.horizontalCenter

                Button
                {
                    width: 80; height: 40
                    text: "LEFT"
                    onPressed: {
                        cpp.onButtonPressed_Left();
                    }

                    onReleased: {
                        cpp.onButtonReleased_Left();
                    }
                }
                Image
                {
                    width: 84; height: 40
                    fillMode: Image.PreserveAspectFit
                    source: "imgs/circle_image.png"
                }
                Button
                {
                    width: 80; height: 40
                    text: "RIGHT"
                    onPressed: {
                        cpp.onButtonPressed_Right();
                    }

                    onReleased: {
                        cpp.onButtonReleased_Right();
                    }
                }
            }



                Button{
                    width: 80; height: 40
                    anchors.top: row_middle.bottom
                    anchors.topMargin: 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "DOWN"
                    onPressed: {
                        cpp.onButtonPressed_Down();
                    }

                    onReleased: {
                        cpp.onButtonReleased_Down();
                    }
                }


    }/* Main column */
}/* Page */

