/**
 * Filer.qml - Native File Manager UI (Qt Quick + Client-Side Decorations)
 * uptc architecture: Qt Quick apps draw their own decorations
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import OpenSEF.AppKit 1.0

Window {
    id: window
    width: 900
    height: 550
    visible: true
    title: "Filer"
    // Client-side decorations (uptc architecture)
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"

    ListModel { id: files }

    // Make window draggable
    MouseArea {
        id: titleBarDragArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 32
        z: 1000
        
        property point clickPos: Qt.point(0, 0)
        
        onPressed: (mouse) => {
            clickPos = Qt.point(mouse.x, mouse.y)
        }
        
        onPositionChanged: (mouse) => {
            if (pressed) {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                window.x += delta.x
                window.y += delta.y
            }
        }
    }

    // --- ROOT CONTAINER ---
    Rectangle {
        id: root
        anchors.fill: parent
        radius: 16 // Ares standard
        color: "transparent"
        clip: true

        // Drop Shadow
        Rectangle {
            anchors.fill: mainContainer
            anchors.margins: -4
            radius: 16
            color: "#403D3320"
            z: -1
        }

        // Main Container with rounded corners
        Rectangle {
            id: mainContainer
            anchors.fill: parent
            radius: 16
            color: "#FAFAF9" // Warm Ceramic
            
            // Custom Titlebar with Traffic Lights
            Rectangle {
                id: customTitleBar
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 32
                radius: 16
                color: "#FAFAF9"
                z: 100
                
                // Traffic Lights (Circular, Ares colors)
                Row {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 12
                    spacing: 8
                    
                    // Close Button (Space Orange)
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: closeMA.containsMouse ? "#FF6347" : "#E85D04"
                        
                        MouseArea {
                            id: closeMA
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: window.close()
                        }
                        
                        // "X" symbol on hover
                        Text {
                            anchors.centerIn: parent
                            text: "×"
                            color: "white"
                            font.pixelSize: 10
                            font.bold: true
                            opacity: closeMA.containsMouse ? 1.0 : 0.0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }
                    
                    // Minimize Button (Mars Gold)
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: minMA.containsMouse ? "#FFE066" : "#D4A93E"
                        
                        MouseArea {
                            id: minMA
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: window.showMinimized()
                        }
                        
                        // "-" symbol on hover
                        Rectangle {
                            width: 6
                            height: 2
                            anchors.centerIn: parent
                            color: "white"
                            opacity: minMA.containsMouse ? 1.0 : 0.0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }
                    
                    // Maximize Button (Mission Blue)
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: maxMA.containsMouse ? "#3395FF" : "#3D5A80"
                        
                        MouseArea {
                            id: maxMA
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (window.visibility === Window.Maximized) {
                                    window.showNormal()
                                } else {
                                    window.showMaximized()
                                }
                            }
                        }
                        
                        // "□" symbol on hover
                        Rectangle {
                            width: 6
                            height: 6
                            anchors.centerIn: parent
                            color: "transparent"
                            border.width: 1.5
                            border.color: "white"
                            opacity: maxMA.containsMouse ? 1.0 : 0.0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }
                }
                
                // Window Title
                Text {
                    anchors.centerIn: parent
                    text: window.title
                    color: "#1A1A1A"
                    font.pixelSize: 13
                    font.family: "Inter"
                    font.weight: Font.DemiBold
                }
            }

            // Content Area (below titlebar)
            RowLayout {
                id: mainRow
                anchors.fill: parent
                anchors.topMargin: 32 // Below titlebar
                spacing: 0
                // Background Compound Shape
                Rectangle { anchors.fill: parent; radius: 16; color: "#F2F2F7" } // Base Rounded
                Rectangle { 
                    anchors.right: parent.right; anchors.top: parent.top; anchors.bottom: parent.bottom; 
                    width: 20; color: "#F2F2F7" // Patch right side to be square
                }
                
                
                // Content Layer
                Item {
                    anchors.fill: parent
                    
                    // Navigation
                    Column {
                        anchors.top: parent.top; anchors.topMargin: 50
                        anchors.left: parent.left; anchors.right: parent.right
                        anchors.leftMargin: 10; anchors.rightMargin: 10
                        spacing: 4

                        Repeater {
                            model: [
                                {label: "Favorites", header: true},
                                {label: "Recents", color: "#4A9FD4", header: false},
                                {label: "Documents", color: "#D4A93E", header: false},
                                {label: "Downloads", color: "#28C840", header: false},
                                {label: "Pictures", color: "#FF5F57", header: false},
                                {label: "Locations", header: true},
                                {label: "VitusHD", color: "#999999", header: false}
                            ]
                            
                            delegate: Rectangle {
                                width: parent.width
                                height: modelData.header ? 24 : 34
                                color: modelData.label === "Documents" ? "#E5E5EA" : "transparent"
                                radius: 6
                                
                                Row {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: modelData.header ? 8 : 12
                                    spacing: 8
                                    
                                    // Procedural Sidebar Icon
                                    Rectangle {
                                        visible: !modelData.header
                                        width: 12; height: 12; radius: 3
                                        color: modelData.color || "black"
                                    }
                                    
                                    Text {
                                        text: modelData.label
                                        font.family: "Inter"
                                        font.pixelSize: modelData.header ? 11 : 13
                                        font.bold: modelData.header
                                        color: modelData.header ? "#888888" : "#333333"
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Separator
            Rectangle { Layout.preferredWidth: 1; Layout.fillHeight: true; color: "#E0E0E0" }

            // === B. CONTENT (Right Rounded) ===
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                // Background Compound Shape
                Rectangle { anchors.fill: parent; radius: 16; color: "#FAFAF9" } // Base Rounded (Ceramic)
                Rectangle { 
                    anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; 
                    width: 20; color: "#FAFAF9" // Patch left side to be square
                }
                
                // Content Layer
                Item {
                    anchors.fill: parent
                    
                    // Toolbar
                    Rectangle {
                        id: contentToolbar
                        height: 50; width: parent.width; color: "transparent"
                        Text {
                            anchors.centerIn: parent
                            text: "Documents"
                            font.family: "Inter"; font.pixelSize: 14; font.bold: true; color: "#333333"
                        }
                    }
                    Rectangle { anchors.top: contentToolbar.bottom; width: parent.width; height: 1; color: "#F0F0F0" }
                    
                    // Grid
                    GridView {
                        id: fileGrid
                        anchors.top: contentToolbar.bottom; anchors.bottom: parent.bottom
                        anchors.left: parent.left; anchors.right: parent.right
                        anchors.margins: 20
                        cellWidth: 100; cellHeight: 110
                        clip: true // Internal clip for grid content remains fine
                        model: files
                        
                        delegate: Item {
                            width: 100; height: 100
                            
                            // PROCEDURAL ICON
                            Item {
                                width: 64; height: 60
                                anchors.centerIn: parent
                                anchors.verticalCenterOffset: -10
                                
                                // FOLDER SHAPE
                                Rectangle {
                                    visible: model.isDir
                                    anchors.fill: parent
                                    color: "#5AA5D8" // Vitus Blue
                                    radius: 4
                                    // Tab
                                    Rectangle {
                                        width: 24; height: 8
                                        color: "#5AA5D8"
                                        anchors.bottom: parent.top
                                        anchors.left: parent.left
                                        radius: 2
                                    }
                                }
                                
                                // FILE SHAPE
                                Rectangle {
                                    visible: !model.isDir
                                    width: 48; height: 60
                                    anchors.centerIn: parent
                                    color: "#FFFFFF"
                                    border.color: "#E0E0E0"
                                    radius: 2
                                    // Dog ear
                                    Rectangle {
                                        width: 12; height: 12
                                        color: "#F0F0F0"
                                        anchors.top: parent.top
                                        anchors.right: parent.right
                                    }
                                }
                            }
                            
                            Text {
                                anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
                                text: model.name
                                font.family: "Inter"; font.pixelSize: 12; color: "#333333"
                                elide: Text.ElideMiddle; width: parent.width - 10; horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: loadFiles()

    function loadFiles() {
        if (files.count > 0) return;
        files.append({name: "Financials", isDir: true})
        files.append({name: "Report.pdf", isDir: false})
        files.append({name: "Design.psd", isDir: false})
        files.append({name: "Notes.txt", isDir: false})
    }
}
