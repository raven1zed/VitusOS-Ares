import QtQuick
import QtQuick.Controls
import QtQuick.Window
import "../../opensef-shell-qt/qml/components"

/**
 * Filer.qml - Native VitusOS File Manager
 * 
 * GNUstep-inspired C++ file manager with Qt Quick UI.
 * Uses FilerController (C++) for real filesystem operations.
 */
ApplicationWindow {
    id: filerWindow
    
    title: "Filer - " + filerController.currentPathName
    width: 900
    height: 600
    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.Window
    
    AresWindow {
        anchors.fill: parent
        title: filerController.currentPathName
        cornerRadius: 20
        backgroundColor: "#FBFBFB"
        targetWindow: filerWindow

        // Main Layout
        Row {
            anchors.fill: parent
            
            // Sidebar
            Rectangle {
                id: sidebar
                width: 200
                height: parent.height
                color: "#F5F5F4"
                
                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 4
                    
                    // Favorites Header
                    Text {
                        text: "Favorites"
                        font.family: "Inter"
                        font.pixelSize: 11
                        font.weight: Font.DemiBold
                        color: "#6B6B6B"
                        leftPadding: 8
                        bottomPadding: 8
                    }
                    
                    // Desktop
                    SidebarItem {
                        label: "Desktop"
                        iconColor: "#3D5A80"
                        onClicked: filerController.goToDesktop()
                    }
                    
                    // Documents
                    SidebarItem {
                        label: "Documents"
                        iconColor: "#3D5A80"
                        onClicked: filerController.goToDocuments()
                    }
                    
                    // Downloads
                    SidebarItem {
                        label: "Downloads"
                        iconColor: "#3D5A80"
                        onClicked: filerController.goToDownloads()
                    }
                    
                    // Pictures
                    SidebarItem {
                        label: "Pictures"
                        iconColor: "#3D5A80"
                        onClicked: filerController.goToPictures()
                    }
                    
                    // Music
                    SidebarItem {
                        label: "Music"
                        iconColor: "#3D5A80"
                        onClicked: filerController.goToMusic()
                    }
                    
                    // Spacer
                    Item { height: 20; width: 1 }
                    
                    // Locations Header
                    Text {
                        text: "Locations"
                        font.family: "Inter"
                        font.pixelSize: 11
                        font.weight: Font.DemiBold
                        color: "#6B6B6B"
                        leftPadding: 8
                        bottomPadding: 8
                    }
                    
                    // Home
                    SidebarItem {
                        label: "Home"
                        iconColor: "#E85D04"
                        onClicked: filerController.goHome()
                    }
                }
            }
            
            // Main Content Area
            Rectangle {
                width: parent.width - sidebar.width
                height: parent.height
                color: "white"
                
                Column {
                    anchors.fill: parent
                    
                    // Toolbar
                    Rectangle {
                        width: parent.width
                        height: 40
                        color: "#FAFAF9"
                        
                        Row {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 12
                            spacing: 8
                            
                            // Back Button
                            ToolbarButton {
                                text: "←"
                                enabled: filerController.canGoBack
                                onClicked: filerController.goBack()
                            }
                            
                            // Forward Button
                            ToolbarButton {
                                text: "→"
                                enabled: filerController.canGoForward
                                onClicked: filerController.goForward()
                            }
                            
                            // Path Display
                            Text {
                                text: filerController.currentPath
                                font.family: "Inter"
                                font.pixelSize: 12
                                color: "#666666"
                                anchors.verticalCenter: parent.verticalCenter
                                elide: Text.ElideMiddle
                                width: 400
                            }
                        }
                    }
                    
                    // Separator
                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#E5E5E5"
                    }
                    
                    // File Grid
                    GridView {
                        id: fileGrid
                        width: parent.width
                        height: parent.height - 41
                        cellWidth: 100
                        cellHeight: 100
                        clip: true
                        
                        model: filerController.fileList
                        
                        delegate: Item {
                            width: 100
                            height: 100
                            
                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: 4
                                radius: 8
                                color: modelData.selected ? "#E3F2FD" : 
                                       (itemMouse.containsMouse ? "#F5F5F5" : "transparent")
                                
                                Column {
                                    anchors.centerIn: parent
                                    spacing: 6
                                    
                                    // Icon
                                    Rectangle {
                                        width: 48
                                        height: 48
                                        radius: 10
                                        color: modelData.isDir ? "#3D5A80" : "#FAFAF9"
                                        border.width: modelData.isDir ? 0 : 1
                                        border.color: "#E5E5E5"
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        
                                        Text {
                                            anchors.centerIn: parent
                                            text: modelData.isDir ? "📁" : "📄"
                                            font.pixelSize: 24
                                        }
                                    }
                                    
                                    // Name
                                    Text {
                                        text: modelData.name
                                        font.family: "Inter"
                                        font.pixelSize: 11
                                        color: "#2D2D2D"
                                        width: 80
                                        horizontalAlignment: Text.AlignHCenter
                                        elide: Text.ElideMiddle
                                        maximumLineCount: 2
                                        wrapMode: Text.WrapAnywhere
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                }
                                
                                MouseArea {
                                    id: itemMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    
                                    onClicked: {
                                        filerController.selectItem(index, mouse.modifiers & Qt.ControlModifier)
                                    }
                                    
                                    onDoubleClicked: {
                                        filerController.openItem(index)
                                    }
                                }
                            }
                        }
                        
                        // Empty state
                        Text {
                            anchors.centerIn: parent
                            text: "Empty folder"
                            font.family: "Inter"
                            font.pixelSize: 14
                            color: "#999999"
                            visible: fileGrid.count === 0
                        }
                    }
                }
            }
        }
    }
    
    // Sidebar item component
    component SidebarItem: Rectangle {
        property string label: ""
        property color iconColor: "#3D5A80"
        signal clicked()
        
        width: parent ? parent.width : 176
        height: 32
        radius: 6
        color: sidebarMouse.containsMouse ? "#EDECEB" : "transparent"
        
        Row {
            anchors.fill: parent
            anchors.leftMargin: 12
            spacing: 10
            
            Rectangle {
                width: 14
                height: 14
                radius: 3
                color: iconColor
                anchors.verticalCenter: parent.verticalCenter
            }
            
            Text {
                text: label
                font.family: "Inter"
                font.pixelSize: 13
                color: "#2D2D2D"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        
        MouseArea {
            id: sidebarMouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: parent.clicked()
        }
    }
    
    // Toolbar button component
    component ToolbarButton: Rectangle {
        property string text: ""
        property bool enabled: true
        signal clicked()
        
        width: 28
        height: 28
        radius: 6
        color: enabled ? (toolMouse.containsMouse ? "#E5E5E5" : "transparent") : "transparent"
        opacity: enabled ? 1.0 : 0.4
        
        Text {
            anchors.centerIn: parent
            text: parent.text
            font.pixelSize: 16
            color: "#333333"
        }
        
        MouseArea {
            id: toolMouse
            anchors.fill: parent
            hoverEnabled: true
            enabled: parent.enabled
            onClicked: parent.clicked()
        }
    }
}
