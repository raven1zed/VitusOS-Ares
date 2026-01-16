/**
 * SystemTrayDropdown.qml - Control Center Dropdown
 * Ares Design System compliant dropdown for system controls
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: dropdown
    
    width: 320
    height: 400
    x: parent.width - width - 16
    y: parent.height + 8
    
    modal: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    
    background: Rectangle {
        color: "#FAFAF9"  // Warm Ceramic
        radius: 16
        border.width: 1
        border.color: "#E0E0E0"  // Cosmic Gray
        
        // Drop shadow
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            color: "#30000000"
            radius: 24
            samples: 49
            verticalOffset: 8
        }
    }
    
    contentItem: ColumnLayout {
        spacing: 0
        anchors.fill: parent
        anchors.margins: 16
        
        // --- NETWORK SECTION ---
        Text {
            text: "Network"
            font.family: "Inter"
            font.pixelSize: 11
            font.weight: Font.Bold
            color: "#808080"  // Cosmic Gray
            Layout.fillWidth: true
            Layout.bottomMargin: 8
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            radius: 12
            color: "#F5F5F5"  // Lunar Gray
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12
                
                // WiFi Icon
                Item {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    
                    // Signal bars
                    Row {
                        anchors.centerIn: parent
                        spacing: 2
                        Rectangle { width: 3; height: 6; color: "#1A1A1A"; radius: 1 }
                        Rectangle { width: 3; height: 10; color: "#1A1A1A"; radius: 1 }
                        Rectangle { width: 3; height: 14; color: "#1A1A1A"; radius: 1 }
                    }
                }
                
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    
                    Text {
                        text: "Wi-Fi"
                        font.family: "Inter"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#1A1A1A"
                    }
                    
                    Text {
                        text: "Connected to Network"
                        font.family: "Inter"
                        font.pixelSize: 12
                        color: "#808080"
                    }
                }
                
                // Toggle switch
                Rectangle {
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 24
                    radius: 12
                    color: "#E57C3A"  // Space Orange (on state)
                    
                    Rectangle {
                        width: 20
                        height: 20
                        radius: 10
                        color: "#FFFFFF"
                        x: parent.width - width - 2
                        y: 2
                        
                        Behavior on x { NumberAnimation { duration: 150 } }
                    }
                }
            }
            
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    console.log("[SystemTray] WiFi toggle clicked")
                    // TODO: Implement via OSFDesktop network manager
                }
            }
        }
        
        Item { Layout.preferredHeight: 12 } // Spacer
        
        // --- VOLUME SECTION ---
        Text {
            text: "Sound"
            font.family: "Inter"
            font.pixelSize: 11
            font.weight: Font.Bold
            color: "#808080"
            Layout.fillWidth: true
            Layout.bottomMargin: 8
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            radius: 12
            color: "#F5F5F5"
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12
                
                // Volume icon
                Item {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    
                    // Speaker shape
                    Canvas {
                        anchors.centerIn: parent
                        width: 20
                        height: 20
                        
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.fillStyle = "#1A1A1A"
                            ctx.beginPath()
                            ctx.moveTo(4, 6)
                            ctx.lineTo(8, 6)
                            ctx.lineTo(12, 2)
                            ctx.lineTo(12, 18)
                            ctx.lineTo(8, 14)
                            ctx.lineTo(4, 14)
                            ctx.closePath()
                            ctx.fill()
                            
                            // Sound waves
                            ctx.lineWidth = 2
                            ctx.strokeStyle = "#1A1A1A"
                            ctx.beginPath()
                            ctx.arc(12, 10, 4, -0.5, 0.5)
                            ctx.stroke()
                            ctx.beginPath()
                            ctx.arc(12, 10, 7, -0.5, 0.5)
                            ctx.stroke()
                        }
                    }
                }
                
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Text {
                            text: "Volume"
                            font.family: "Inter"
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "#1A1A1A"
                            Layout.fillWidth: true
                        }
                        
                        Text {
                            text: "65%"
                            font.family: "Inter"
                            font.pixelSize: 12
                            color: "#808080"
                        }
                    }
                    
                    // Volume slider
                    Slider {
                        Layout.fillWidth: true
                        from: 0
                        to: 100
                        value: 65
                        
                        background: Rectangle {
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: parent.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#E0E0E0"
                            
                            Rectangle {
                                width: parent.width * (parent.parent.value / parent.parent.to)
                                height: parent.height
                                radius: 2
                                color: "#E57C3A"  // Space Orange
                            }
                        }
                        
                        handle: Rectangle {
                            x: parent.leftPadding + parent.visualPosition * parent.availableWidth - width / 2
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 8
                            color: "#FFFFFF"
                            border.width: 2
                            border.color: "#E57C3A"
                        }
                    }
                }
            }
        }
        
        Item { Layout.preferredHeight: 12 }
        
        // --- BATTERY SECTION (if applicable) ---
        Text {
            text: "Battery"
            font.family: "Inter"
            font.pixelSize: 11
            font.weight: Font.Bold
            color: "#808080"
            Layout.fillWidth: true
            Layout.bottomMargin: 8
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            radius: 12
            color: "#F5F5F5"
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12
                
                // Battery icon
                Item {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    
                    Rectangle {
                        width: 24
                        height: 14
                        anchors.centerIn: parent
                        radius: 3
                        color: "transparent"
                        border.width: 2
                        border.color: "#1A1A1A"
                        
                        Rectangle {
                            width: 14
                            height: 10
                            radius: 2
                            color: "#28C840"  // Green for charged
                            anchors.left: parent.left
                            anchors.leftMargin: 2
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    
                    Rectangle {
                        width: 3
                        height: 6
                        radius: 1.5
                        color: "#1A1A1A"
                        anchors.left: parent.left
                        anchors.leftMargin: 28
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    
                    Text {
                        text: "Battery"
                        font.family: "Inter"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#1A1A1A"
                    }
                    
                    Text {
                        text: "75% • 3:24 remaining"
                        font.family: "Inter"
                        font.pixelSize: 12
                        color: "#808080"
                    }
                }
            }
        }
        
        Item { Layout.fillHeight: true } // Spacer to push buttons down
        
        // --- QUICK ACTIONS ---
        Row {
            Layout.fillWidth: true
            spacing: 8
            
            Button {
                width: (parent.width - 8) / 2
                height: 36
                
                contentItem: Text {
                    text: "Lock Screen"
                    font.family: "Inter"
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: "#1A1A1A"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                background: Rectangle {
                    radius: 8
                    color: parent.hovered ? "#F0F0F0" : "#F5F5F5"
                    border.width: 1
                    border.color: "#E0E0E0"
                    
                    Behavior on color { ColorAnimation { duration: 150 } }
                }
                
                onClicked: {
                    console.log("[SystemTray] Lock screen clicked")
                    dropdown.close()
                    // TODO: Implement via OSFDesktop session manager
                }
            }
            
            Button {
                width: (parent.width - 8) / 2
                height: 36
                
                contentItem: Text {
                    text: "Settings"
                    font.family: "Inter"
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: "#1A1A1A"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                background: Rectangle {
                    radius: 8
                    color: parent.hovered ? "#F0F0F0" : "#F5F5F5"
                    border.width: 1
                    border.color: "#E0E0E0"
                    
                    Behavior on color { ColorAnimation { duration: 150 } }
                }
                
                onClicked: {
                    console.log("[SystemTray] Settings clicked")
                    dropdown.close()
                    // TODO: Launch Settings app via OSFDesktop
                }
            }
        }
    }
    
    enter: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: 150
            easing.type: Easing.OutCubic
        }
        NumberAnimation {
            property: "y"
            from: dropdown.y - 10
            to: dropdown.y
            duration: 150
            easing.type: Easing.OutCubic
        }
    }
    
    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: 150
            easing.type: Easing.InCubic
        }
    }
}
