import QtQuick 2.8
import QtQuick.Controls 2.1
import QmlMainClass 1.0
import QtQuick.Window 2.1


ApplicationWindow
{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Sophia APP Controller")

    property string statusText: "Waiting for actions..."
    property string connection_status_label: "NOT connected!"

    property string ip_address: "192.168.0.1"
    property int port: 1337

    QmlMainClass
    {
        id: cpp
    }

    header: Rectangle
    {
        Rectangle
        {
            height: 10; width: parent.height
        }

        id: header
        Image
        {
            height: 120
            anchors.verticalCenterOffset: 60
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit; clip: true
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "imgs/sophia_logo.png"

            MouseArea {
                 anchors.fill: parent
                 onClicked: {
                     cpp.test()
                 }
            }
        }
    }

    SwipeView
    {
        id: swipeView
        anchors.bottomMargin: 36
        anchors.topMargin: 122
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        clip: true
        interactive: false

        ViewAuto
        {
            id: root_auto
        }

        ViewManual
        {
            id: root_manual
        }

        ViewConnection
        {
            id: root_connection
        }
    }


    Rectangle
    {
        height: 30; width: root.width
        color: "green"
        anchors.bottom: tabBar.top
        anchors.bottomMargin: 10
        Text {
            id: status_text
            anchors.centerIn: parent
            text: qsTr(statusText)
        }
    }

    footer: TabBar
    {
        id: tabBar
        currentIndex: swipeView.currentIndex

        property color tab_selected: "#627782"
        property color tab_unselected: "#c8ccce"
        property int tabs_height: 45

        TabButton
        {
            id: button_tab_auto
            text: qsTr("Auto")
            height: tabBar.tabs_height
            background: Rectangle
            {
                color: tabBar.currentIndex === 0 ? tabBar.tab_selected : tabBar.tab_unselected
            }
            icon.name: "tab_auto"
            icon.source: "imgs/tabs/baseline_ballot_black_24dp.png"
        }
        TabButton
        {
            id: button_tab_manual
            text: qsTr("Manual")
            height: tabBar.tabs_height
            background: Rectangle
            {
                color: tabBar.currentIndex === 1 ? tabBar.tab_selected : tabBar.tab_unselected
            }
            icon.name: "tab_manual"
            icon.source: "imgs/tabs/baseline_control_camera_black_24dp.png"
        }
        TabButton
        {
            id: button_tab_connection
            text: qsTr("Connection")
            height: tabBar.tabs_height
            background: Rectangle
            {
                color: tabBar.currentIndex === 2 ? tabBar.tab_selected : tabBar.tab_unselected
            }
            icon.name: "tab_connection"
            icon.source: "imgs/tabs/baseline_wifi_black_24dp.png"
        }
    }
}
