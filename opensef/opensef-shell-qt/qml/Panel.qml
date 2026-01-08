/**
 * Panel.qml - Global Menu Bar
 * 
 * VitusOS Ares top panel with:
 * - Multitask button (Mars Orange, 80px)
 * - Global menu items
 * - Active window title
 * - System tray (right)
 * - Clock
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: panel
    
    height: 28
    color: "#F5F5F5"
    
    // Drop shadow for depth
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        color: "#30000000"
        radius: 4
        samples: 9
        verticalOffset: 2
    }
    
    Row {
        id: leftRow
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8
        
        // Multitask Button (80px, Mars Orange)
        Rectangle {
            id: multitaskButton
            width: 80
            height: 20
            radius: 4
            color: multitaskMouseArea.containsMouse ? "#E07337" : "#D4622A"
            
            anchors.verticalCenter: parent.verticalCenter
            
            Text {
                anchors.centerIn: parent
                text: "Multitask"
                color: "white"
                font.pixelSize: 11
                font.bold: true
                font.family: "Inter"
            }
            
            MouseArea {
                id: multitaskMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: panelController.toggleMultitask()
            }
            
            Behavior on color {
                ColorAnimation { duration: 150 }
            }
        }
        
        // Global Menu Items
        Repeater {
            model: panelController.globalMenuItems
            
            delegate: Rectangle {
                width: menuLabel.implicitWidth + 16
                height: 20
                radius: 4
                color: menuTitleMouse.containsMouse ? "#20000000" : "transparent"
                anchors.verticalCenter: parent.verticalCenter
                
                Text {
                    id: menuLabel
                    anchors.centerIn: parent
                    text: modelData.title
                    color: "#1A1A1A"
                    font.pixelSize: 13
                    font.bold: true
                    font.family: "Inter"
                }
                
                MouseArea {
                    id: menuTitleMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: panelController.showMenu(index)
                }
                
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
            }
        }
        
        // Active Window Title
        Text {
            text: panelController.activeWindowTitle
            color: "#666666"
            font.pixelSize: 13
            font.family: "Inter"
            anchors.verticalCenter: parent.verticalCenter
            leftPadding: 12
            
            elide: Text.ElideRight
            maximumLineCount: 1
        }
    }
    
    // Right side: System Tray + Clock
    Row {
        id: rightRow
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        spacing: 12
        
        SystemTray {
            anchors.verticalCenter: parent.verticalCenter
        }
        
        // Clock
        Text {
            text: panelController.currentTime
            color: "#1A1A1A"
            font.pixelSize: 12
            font.family: "Inter"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
