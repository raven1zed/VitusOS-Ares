import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: dockRoot
    
    width: parent.width
    height: 120 // Increased to allow magnification space

    // Hit Area for Dock Reveal
    MouseArea {
        id: globalMouseArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
    }
    
    // Auto-hide logic
    property bool isShown: globalMouseArea.containsMouse || !dockController.isHidden
    
    // DOCK CONTAINER (Controls position)
    Item {
        id: dockContainer
        height: 100
        width: dockRow.width + 48
        anchors.horizontalCenter: parent.horizontalCenter
        
        // Push reveal logic
        y: isShown ? parent.height - height - 10 : parent.height - 20 // visible vs peek
        
        Behavior on y { 
            NumberAnimation { 
                duration: 300; easing.type: Easing.OutCubic 
            } 
        }

        // BACKGROUND PILL (Behind icons)
        Rectangle {
            id: dockBackground
            anchors.fill: parent
            anchors.bottomMargin: 20 // Leave space for dots below!
            radius: 20
            color: "#E0FAFAF9" // Translucent Ceramic
            border.width: 1
            border.color: "#20000000"
            antialiasing: true
        }

        // ICONS ROW
        Row {
            id: dockRow
            anchors.centerIn: dockBackground
            spacing: 12
            
            Repeater {
                model: dockController.dockItems
                
                delegate: Item {
                    id: iconDelegate
                    width: baseSize * targetScale
                    height: baseSize * targetScale
                    
                    property real baseSize: 48
                    property real targetScale: iconMA.containsMouse ? 1.4 : 1.0
                    
                    // Smooth scale animation
                    Behavior on targetScale { 
                        NumberAnimation { 
                            duration: 150
                            easing.type: Easing.OutCubic 
                        } 
                    }

                    // ICON IMAGE
                    Image {
                        width: parent.width
                        height: parent.height
                        anchors.centerIn: parent
                        source: Qt.resolvedUrl("../../../assets/icons/" + modelData.name.toLowerCase() + ".svg")
                        sourceSize: Qt.size(128, 128)
                        smooth: true
                        antialiasing: true
                        mipmap: true // Fixes downscaling aliasing
                    }
                    
                    // TOOLTIP
                    Rectangle {
                        visible: iconMA.containsMouse
                        width: tooltipText.contentWidth + 16
                        height: 24
                        radius: 6
                        color: "#CC000000"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.top
                        anchors.bottomMargin: 8
                        
                        Text {
                            id: tooltipText
                            anchors.centerIn: parent
                            text: modelData.name
                            color: "white"
                            font.pixelSize: 12
                        }
                    }

                    // RUNNING DOT (OUTSIDE THE PILL)
                    Rectangle {
                        visible: modelData.running
                        width: 4; height: 4; radius: 2
                        color: "#E85D04" // Space Orange
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.bottom // Attach to bottom of ICON (which is centered in pill)
                        anchors.topMargin: 24
                    }

                    MouseArea {
                        id: iconMA
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: dockController.launchApp(index)
                    }
                }
            }
        }
    }
}
