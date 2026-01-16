import QtQuick 2.15
import QtQuick.Controls 2.15
import "../../opensef-shell-qt/qml/components"

/**
 * Filer.qml - Native VitusOS File Manager
 * 
 * Re-aligned with Ares Design System using standardized components.
 * This ensures perfect 20px corners and "openSEF UI sake" decorations.
 */
AresWindow {
    id: filerWindow
    
    title: "Filer"
    width: 900
    height: 600
    cornerRadius: 20
    backgroundColor: "#FBFBFB" // Soft Cream

    // Sidebar + File Grid layout
    Row {
        anchors.fill: parent
        
        // Sidebar (Frosted Glass influence)
        Rectangle {
            id: sidebar
            width: 200
            height: parent.height
            color: "#F5F5F4" // Muted Ceramic
            
            Column {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 4
                
                Text {
                    text: "Favorites"
                    font.family: "Inter"
                    font.pixelSize: 11
                    font.weight: Font.DemiBold
                    color: "#6B6B6B"
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    bottomPadding: 8
                }
                
                Repeater {
                    model: ["Desktop", "Documents", "Downloads", "Pictures", "Music"]
                    delegate: Rectangle {
                        width: parent.width
                        height: 32
                        radius: 6
                        color: mouseArea.containsMouse ? "#EDECEB" : "transparent"
                        
                        Row {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            spacing: 10
                            
                            // Placeholder Icon
                            Rectangle {
                                width: 14; height: 14; radius: 3
                                color: "#3D5A80" // Mission Blue
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            
                            Text {
                                text: modelData
                                font.family: "Inter"
                                font.pixelSize: 13
                                color: "#2D2D2D"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }
                }
            }
        }
        
        // Main Content Area
        Rectangle {
            width: parent.width - sidebar.width
            height: parent.height
            color: "white"
            
            GridView {
                id: fileGrid
                anchors.fill: parent
                anchors.margins: 20
                cellWidth: 100
                cellHeight: 120
                model: 24 // Dummy data
                
                delegate: Item {
                    width: 100
                    height: 120
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 8
                        
                        // File Icon
                        Rectangle {
                            width: 64; height: 64; radius: 12
                            color: "#FAFAF9"
                            border.width: 1
                            border.color: "#E5E5E5"
                            anchors.horizontalCenter: parent.horizontalCenter
                            
                            Image {
                                anchors.centerIn: parent
                                width: 40; height: 40
                                sourceSize: Qt.size(80, 80)
                                opacity: 0.8
                                // source: "../../assets/icons/file.svg" (Path placeholder)
                            }
                        }
                        
                        Text {
                            text: "Document " + index
                            font.family: "Inter"
                            font.pixelSize: 12
                            color: "#2D2D2D"
                            width: 80
                            horizontalAlignment: Text.AlignHCenter
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }
}
