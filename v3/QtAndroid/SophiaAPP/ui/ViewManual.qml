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
            id: column_sensors
            anchors.top: gap.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25

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
            id: column_manuals
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: column_sensors.bottom
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

