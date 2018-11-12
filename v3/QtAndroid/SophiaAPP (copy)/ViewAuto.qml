import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 600
    height: 400

    Rectangle{id: gap; width: 10; height: 20;}


    RowLayout {

        anchors.top: gap.bottom
        id: size_fields
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

//    GridLayout
//    {
//        id: grid_layout

//        rows: 2
//        width: 301
//        height: 133
//        anchors.top: parent.top
//        anchors.topMargin: 134
//        anchors.verticalCenterOffset: 20
//        anchors.centerIn: parent
//        flow: GridLayout.TopToBottom


//        GridLayout
//        {
//            rows: 1
//            anchors.centerIn: parent
//            Button
//            {

//                id: button_auto_reset
//                anchors.top: parent.top
//                anchors.centerIn: parent
//                width: 40
//                height: 10
//                text: qsTr("Reset")
//            }
//        }

//        GridLayout
//        {
//            rows: 2
//            anchors.topMargin: 10
//            anchors.centerIn: parent
//            anchors.top: button_auto_reset.bottom

//            Button {
//                id: button_auto_stop
//                width: 20
//                height: 10
//                text: qsTr("Stop")

//                //onClicked: signal test_sig()
//            }

//            Button {
//                id: button_auto_start
//                width: 20
//                height: 10
//                text: qsTr("Start")
//            }
    //        }

    //    }
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
                width: 140; height: 50
                icon.source: "resources/imgs/auto_frame/reset.png"
            }
            Button
            {
                id: button_moveto
                text: qsTr("Move to")
                width: 140; height: 50
                icon.source: "resources/imgs/auto_frame/moveto.png"
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
                width: 140; height: 50
                icon.source: "resources/imgs/auto_frame/baseline_lock_black_24dp.png"
            }
            Button
            {
                id: button_unlock
                text: qsTr("Unlock")
                width: 140; height: 50
                icon.source: "resources/imgs/auto_frame/baseline_lock_open_black_24dp.png"
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
                icon.source: "resources/imgs/auto_frame/cut.png"
            }
        }
    }

}

