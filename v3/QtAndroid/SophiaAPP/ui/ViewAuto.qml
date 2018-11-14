import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QmlMainClass 1.0

Page {
    id: page
    width: 600
    height: 400

    Rectangle{id: gap; width: 10; height: 20;}

    QmlMainClass
    {
        id: cpp
    }

    RowLayout {

        anchors.top: gap.bottom
        id: size_fields
        width: 300
        anchors.horizontalCenter: parent.horizontalCenter


        TextField {
            id: input_size_field
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

    Column
    {
        id: column_buttons
        anchors.top: size_fields.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        Row
        {
            id: buttons_row_reset_moveto
            anchors.horizontalCenter: parent.horizontalCenter // The "Row" type lays out its child items in a horizontal line
            spacing: 10 // Places 20px of space between items

            Button
            {
                id: button_reset
                text: qsTr("RESET")
                width: 145; height: 50
                icon.source: "imgs/auto_frame/reset.png"

                onPressed: {
                    cpp.onButtonPressed_Reset();
                }
            }
            Button
            {
                id: button_moveto
                text: qsTr("Move to")
                width: 145; height: 50
                icon.source: "imgs/auto_frame/moveto.png"

                onPressed: {
                    cpp.onButtonPressed_MoveTo( parseInt(input_size_field.getText(0, 4)) );
                }
            }
        }

        Row
        {
            id: buttons_row_lock_unlock
            anchors.horizontalCenter: parent.horizontalCenter // The "Row" type lays out its child items in a horizontal line
            spacing: 10 // Places 20px of space between items
            anchors.top: buttons_row_reset_moveto.bottom
            anchors.topMargin: 10

            Button
            {
                id: button_lock
                text: qsTr("Lock")
                width: 145; height: 50
                icon.source: "imgs/auto_frame/baseline_lock_black_24dp.png"

                onPressed: {
                    cpp.onButtonPressed_Lock();
                }
            }
            Button
            {
                id: button_unlock
                text: qsTr("Unlock")
                width: 145; height: 50
                icon.source: "imgs/auto_frame/baseline_lock_open_black_24dp.png"
                onPressed: {
                    cpp.onButtonPressed_Unlock();
                }
            }
        }

        Row
        {
            id: buttons_row_cut
            anchors.horizontalCenter: parent.horizontalCenter // The "Row" type lays out its child items in a horizontal line
            spacing: 10 // Places 20px of space between items
            anchors.top: buttons_row_lock_unlock.bottom
            anchors.topMargin: 10

            Button
            {
                id: button_cut
                text: qsTr("Cut")
                width: 300; height: 50
                icon.source: "imgs/auto_frame/cut.png"

                onPressed: {
                    cpp.onButtonPressed_Cut();
                }
            }
        }
    }

}

