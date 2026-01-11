/**
 * AresPanel.qml - Global Menu Bar
 * 
 * EXACT match to UI_DESIGN.md mockup:
 * - Orange vertical bar (full height, 8px wide) on LEFT
 * - Global menus: Filer, Edit, View, Help (NO window title!)
 * - Clock on RIGHT: "Tuesday,12 10:15 PM" format
 * - Translucent background
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: panel
    
    height: 28
    color: Qt.rgba(0.96, 0.96, 0.96, 0.95)  // Translucent Lunar Gray
    
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        color: "#20000000"
        radius: 12
        samples: 25
        verticalOffset: 1
    }
    
    // Orange Accent - FULL HEIGHT BOX (wider for prominence)
    Rectangle {
        id: accentBox
        width: 20  // Wider box, not thin strip
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "#D4622A"  // Mars Orange
        
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: multitaskController.toggle()
        }
    }
    
    // Global Menu Items
    Row {
        id: menuRow
        anchors.left: accentBox.right
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        spacing: 20
        
        Repeater {
            model: panelController.globalMenuItems
            
            delegate: Text {
                text: modelData.title
                color: "#1A1A1A"
                font.pixelSize: 13
                font.family: "Inter"
                font.weight: index === 0 ? Font.Bold : Font.Normal
                
                MouseArea {
                    id: ma
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        var globalPos = ma.mapToItem(null, 0, 0)
                        panelController.showMenu(index, globalPos.x, globalPos.y + 28)
                    }
                }
            }
        }
    }

    // Active Window Title - REMOVED PER USER COMPLAINT (No "VitusOS Ares" in center)
    
    // Center: Active Window Title
    Text {
        anchors.centerIn: parent
        text: panelController.activeWindowTitle
        color: "#1A1A1A"
        font.pixelSize: 13
        font.family: "Inter"
        font.weight: Font.Bold
        elide: Text.ElideRight
        maximumLineCount: 1
        width: parent.width * 0.4
        horizontalAlignment: Text.AlignHCenter
    }

    // Right Side: System Tray + Clock
    Row {
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        spacing: 16
        
        SystemTray {
            id: systemTray
            anchors.verticalCenter: parent.verticalCenter
        }

        // Clock & Date Container
        Column {
            anchors.verticalCenter: parent.verticalCenter
            spacing: -2
            
            Text {
                id: clock
                text: panelController.currentTime.split(",")[2].trim() // Extract time part: "h:mm:ss AP"
                color: "#1A1A1A"
                font.pixelSize: 13
                font.family: "Inter"
                font.weight: Font.Bold
                anchors.right: parent.right
            }
            
            Text {
                id: dateLabel
                text: panelController.currentTime.split(",")[0] + "," + panelController.currentTime.split(",")[1] // "Sunday, Jan 11"
                color: "#555555"
                font.pixelSize: 9
                font.family: "Inter"
                anchors.right: parent.right
            }
        }
    }
}
