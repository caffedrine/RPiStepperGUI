import QtQuick 2.8
import QtQuick.Controls 2.1


ApplicationWindow
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Sophia APP Controller")


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
            source: "resources/imgs/sophia_logo.png"
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
        }

        ViewManual
        {
        }

        ViewConnection
        {
        }
    }

    footer: TabBar
    {
        id: tabBar
        currentIndex: swipeView.currentIndex

        property color tab_selected: "#627782"
        property color tab_unselected: "#c8ccce"
        property int tabs_height: 40

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
            icon.source: "resources/imgs/tabs/baseline_ballot_black_24dp.png"
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
            icon.source: "resources/imgs/tabs/baseline_control_camera_black_24dp.png"
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
            icon.source: "resources/imgs/tabs/baseline_wifi_black_24dp.png"
        }
    }
}
