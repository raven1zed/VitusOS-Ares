/**
 * Filer.qml - Native File Manager
 * 
 * Uses AresWindow standard (4-corner 20px rounding)
 * Deeply integrated with GNUstep AppKit logic
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import OpenSEF.AppKit 1.0
import "../../opensef-shell-qt/qml/components"

AresWindow {
    id: window
    width: 900
    height: 600
    aresTitle: "Filer"
    
    // Window Content
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        // Universal Window Decorations
        WindowDecorations {
            id: titleBar
            Layout.fillWidth: true
            height: 48
            windowTitle: window.aresTitle
            isActive: window.active
            targetWindow: window
            z: 1000
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#E0E0DF"
        }
        
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0
            
            // Sidebar (Finder-inspired)
            Rectangle {
                Layout.preferredWidth: 230
                Layout.fillHeight: true
                color: "#F2F2F0" 
                
                Rectangle {
                    anchors.right: parent.right
                    width: 1; height: parent.height
                    color: "#E0E0DF"
                }
                
                Column {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    anchors.topMargin: 20
                    spacing: 24
                    
                    Text {
                        text: "FAVORITES"
                        font.family: window.fontFamily
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        font.letterSpacing: 1
                        color: "#8C8C8A"
                    }
                    
                    // Sidebar List
                    Column {
                        width: parent.width
                        spacing: 2
                        
                        Repeater {
                            model: [
                                { name: "Recents", icon: "#415A77" },
                                { name: "Documents", icon: "#D4A93E" },
                                { name: "Downloads", icon: "#2A9D8F" },
                                { name: "Pictures", icon: "#E76F51" }
                            ]
                            
                            delegate: Rectangle {
                                width: parent.width
                                height: 32
                                radius: 8
                                color: index === 1 ? "#E0E0E0" : "transparent"
                                
                                Row {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    spacing: 12
                                    
                                    Rectangle {
                                        width: 14; height: 14; radius: 4; color: modelData.icon
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    
                                    Text {
                                        text: modelData.name
                                        font.family: window.fontFamily
                                        font.pixelSize: 13
                                        font.weight: index === 1 ? Font.DemiBold : Font.Normal
                                        color: "#1A1A1A"
                                        anchors.verticalCenter: parent.verticalCenter
                                        renderType: Text.NativeRendering
                                    }
                                }
                                
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("Sidebar clicked:", modelData.name)
                                    }
                                }
                            }
                        }
                    }
                    
                    Text {
                        text: "LOCATIONS"
                        font.family: window.fontFamily
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        font.letterSpacing: 1
                        color: "#8C8C8A"
                    }
                    
                    Column {
                        width: parent.width
                        spacing: 2
                        
                        Rectangle {
                            width: parent.width
                            height: 32
                            radius: 8
                            color: "transparent"
                            
                            Row {
                                anchors.fill: parent
                                anchors.leftMargin: 12
                                spacing: 12
                                
                                Rectangle {
                                    width: 14; height: 14; radius: 4; color: "#A0A09F"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                
                                Text { 
                                    text: "VitusHD"
                                    font.family: window.fontFamily
                                    font.pixelSize: 13 
                                    color: "#1A1A1A" 
                                    anchors.verticalCenter: parent.verticalCenter
                                    renderType: Text.NativeRendering
                                }
                            }
                        }
                    }
                }
            }
            
            // File View Area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "white"
                
                Column {
                    anchors.centerIn: parent
                    spacing: 16
                    
                    // Empty state visual (NO EMOJIS - USE SHAPES)
                    Rectangle {
                        width: 120; height: 120
                        radius: 60
                        color: "#FAFAF9"
                        border.width: 1
                        border.color: "#E0E0DF"
                        anchors.horizontalCenter: parent.horizontalCenter
                        
                        // Folder Icon Replacement
                        Rectangle {
                            width: 60; height: 40; radius: 4; color: "#D4A93E"
                            anchors.centerIn: parent
                            
                            // Flap
                            Rectangle {
                                width: 20; height: 8; radius: 2; color: "#D4A93E"
                                anchors.bottom: parent.top
                                anchors.left: parent.left
                            }
                        }
                    }

                    Text {
                        text: "No items in Documents"
                        color: "#1A1A1A"
                        font.pixelSize: 18
                        font.weight: Font.Medium
                        font.family: window.fontFamily
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    
                    Text {
                        text: "Files you add will appear here"
                        color: "#8C8C8A"
                        font.pixelSize: 14
                        font.family: window.fontFamily
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
}
