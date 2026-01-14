/**
 * Multitask.qml - Mission Control / Expose View
 * 
 * Fusion Design:
 * - Fluid grid of live window previews
 * - Large, centered tiles
 * - Integrated decorations on tiles
 * - Smooth entrance/exit animations
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects 
import "./components"

Item {
    id: root
    anchors.fill: parent
    visible: false
    opacity: 0
    
    // Background Blur/Dim
    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: 0.4
    }
    
    // STAGE MANAGER SIDE STRIP (Fusion Left)
    Item {
        id: stageStripContainer
        width: 120
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 40
        anchors.leftMargin: 20
        visible: root.visible

        Rectangle {
            id: stageStripMask
            anchors.fill: parent
            radius: 24
            color: "black"
            visible: false
        }

        Rectangle {
            id: stageStrip
            anchors.fill: parent
            radius: 24
            color: "#20FFFFFF"
            
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: stageStripMask
            }
            
            Column {
                anchors.fill: parent
                anchors.topMargin: 20
                spacing: 20
                
                Repeater {
                    model: Math.min(multitaskController.windows.length, 4)
                    delegate: Item {
                        width: 80; height: 60
                        anchors.horizontalCenter: parent.horizontalCenter
                        
                        Rectangle {
                            anchors.fill: parent
                            radius: 12
                            color: "#CCFAFAF9"
                            border.width: 1
                            border.color: "#30FFFFFF"
                            
                            // 4-corner Rounding for small icon previews
                            layer.enabled: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle { width: 80; height: 60; radius: 12 }
                            }

                            Image {
                                anchors.centerIn: parent
                                width: 32; height: 32
                                source: "image://icon/" + multitaskController.windows[index].appId
                            }
                        }
                        
                        rotation: -15
                        scale: 0.8
                    }
                }
            }
        }
        
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            radius: 16
            samples: 24
            color: "#40000000"
        }
        
        NumberAnimation on x { from: -140; to: 20; duration: 400; easing.type: Easing.OutBack }
    }

    // MISSION CONTROL HEADER
    Text {
        text: "Activities"
        font.family: "Inter"
        font.pixelSize: 42
        font.weight: Font.ExtraBold
        color: "white"
        anchors.top: parent.top
        anchors.left: stageStripContainer.right
        anchors.leftMargin: 60
        anchors.topMargin: 60
        opacity: root.visible ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }
    
    // PANORAMIC WINDOW STRIP (Win8 Style)
    ListView {
        id: windowList
        anchors.left: stageStripContainer.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 180
        orientation: ListView.Horizontal
        spacing: 60
        leftMargin: 60
        rightMargin: 100
        model: multitaskController.windows
        snapMode: ListView.SnapToItem
        highlightFollowsCurrentItem: true
        
        delegate: Item {
            id: windowDelegate
            width: 520; height: 380
            
            property bool isHovered: mouseArea.containsMouse
            scale: isHovered ? 1.05 : 1.0
            Behavior on scale { NumberAnimation { duration: 250; easing.type: Easing.OutBack } }
            
            // Mirror reflection effect (Fusion Polish)
            Rectangle {
                anchors.top: cardContainer.bottom
                anchors.topMargin: 8
                anchors.horizontalCenter: cardContainer.horizontalCenter
                width: cardContainer.width
                height: cardContainer.height * 0.25
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#60FFFFFF" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
                opacity: 0.2
                rotation: 180
            }

            // Window Preview Card (Standardized 20px Mask)
            Item {
                id: cardContainer
                width: 520; height: 360
                
                Rectangle {
                    id: cardMask
                    anchors.fill: parent
                    radius: 20
                    color: "black"
                    visible: false
                }
                
                Rectangle {
                    id: card
                    anchors.fill: parent
                    radius: 20
                    color: "#EEFAFAF9"
                    
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: cardMask
                    }
                    
                    // Integrated Window Decorations (Fusion Style)
                    WindowDecorations {
                        id: deco
                        height: 40
                        radius: 20
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        windowTitle: modelData.title
                        isActive: true
                        showTrafficLights: true
                        enabled: false
                        z: 10
                    }
                    
                    // Content Preview Area
                    Rectangle {
                        anchors.top: deco.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        color: "#F8FFFFFF"
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 32
                            
                            Image {
                                width: 128; height: 128
                                source: "image://icon/" + modelData.appId
                                sourceSize: Qt.size(512, 512)
                                fillMode: Image.PreserveAspectFit
                                anchors.horizontalCenter: parent.horizontalCenter
                                smooth: true
                            }
                            
                            Text {
                                text: modelData.name
                                font.family: "Inter"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                color: "#1A1A1A"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
    
                            Text {
                                text: modelData.appId
                                font.family: "Inter"
                                font.pixelSize: 14
                                color: "#E85D04" // Space Orange highlight
                                font.weight: Font.DemiBold
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
                
                // Outer focus border (outside the mask)
                Rectangle {
                    anchors.fill: parent
                    radius: 20
                    color: "transparent"
                    border.width: isHovered ? 4 : 0
                    border.color: "#E85D04"
                    Behavior on border.width { NumberAnimation { duration: 150 } }
                }
                
                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 24
                    radius: 48
                    samples: 40
                    color: "#A0000000"
                }
            }
            
            MouseArea {
                id: mouseArea
                anchors.fill: cardContainer
                hoverEnabled: true
                onClicked: {
                    multitaskController.activateWindow(modelData.windowId)
                    root.close()
                }
            }
        }
        
        // Dynamic Entrance (Fluid Mission Control style)
        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 600; easing.type: Easing.OutCubic }
            NumberAnimation { property: "scale"; from: 0.7; to: 1.0; duration: 600; easing.type: Easing.OutBack }
            NumberAnimation { property: "x"; from: 800; duration: 600; easing.type: Easing.OutBack }
        }
    }
    
    // Show/Hide Logic
    function open() {
        visible = true
        opacity = 1
        multitaskController.refreshWindows()
    }
    
    function close() {
        opacity = 0
        visible = false
    }
    
    Behavior on opacity { NumberAnimation { duration: 200 } }
}
