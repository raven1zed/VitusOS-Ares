/**
 * MessageBox.qml - Unified Dialog/Pop-up Component
 * 
 * All message boxes, alerts, and confirmations use this component
 * for consistent Ares Design Language appearance.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    id: dialog
    
    // Properties
    property string title: ""
    property string message: ""
    property string detailedText: ""
    property string iconType: "info"  // "info", "warning", "error", "question"
    property var buttons: ["OK"]       // ["OK"], ["OK", "Cancel"], ["Yes", "No"], etc.
    property int defaultButton: 0
    property int escapeButton: -1      // Button to trigger on Escape (-1 = none)
    
    // Signals
    signal buttonClicked(int index, string label)
    signal accepted()
    signal rejected()
    
    // Window settings
    width: Math.max(360, contentColumn.implicitWidth + 48)
    height: contentColumn.implicitHeight + 80
    minimumWidth: 300
    minimumHeight: 150
    flags: Qt.Dialog | Qt.FramelessWindowHint
    modality: Qt.ApplicationModal
    color: "transparent"
    
    // Center on parent/screen
    Component.onCompleted: centerOnScreen()
    
    function centerOnScreen() {
        x = (Screen.width - width) / 2
        y = (Screen.height - height) / 2
    }
    
    // Show and return result
    function open() {
        visible = true
        forceActiveFocus()
    }
    
    // Main container
    Rectangle {
        id: container
        anchors.fill: parent
        anchors.margins: 8  // Space for shadow
        radius: 16
        color: "#FAFAF9"  // Warm Ceramic
        
        // Drop shadow
        Rectangle {
            anchors.fill: parent
            anchors.margins: -4
            radius: 16
            color: "#40000000"
            z: -1
        }
        
        // Content
        ColumnLayout {
            id: contentColumn
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16
            
            // Icon + Title row
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                // Icon
                Rectangle {
                    width: 40
                    height: 40
                    radius: 20
                    color: iconColor()
                    visible: iconType !== ""
                    
                    Text {
                        anchors.centerIn: parent
                        text: iconSymbol()
                        font.pixelSize: 20
                        font.bold: true
                        color: "white"
                    }
                }
                
                // Title
                Text {
                    text: dialog.title
                    font.family: "Inter"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    color: "#1A1A1A"
                    Layout.fillWidth: true
                    visible: dialog.title !== ""
                }
            }
            
            // Message
            Text {
                Layout.fillWidth: true
                text: dialog.message
                font.family: "Inter"
                font.pixelSize: 14
                color: "#333333"
                wrapMode: Text.WordWrap
                visible: dialog.message !== ""
            }
            
            // Detailed text (smaller, scrollable)
            ScrollView {
                Layout.fillWidth: true
                Layout.preferredHeight: Math.min(detailedTextItem.implicitHeight, 100)
                visible: dialog.detailedText !== ""
                clip: true
                
                Text {
                    id: detailedTextItem
                    width: parent.width
                    text: dialog.detailedText
                    font.family: "Inter"
                    font.pixelSize: 12
                    color: "#666666"
                    wrapMode: Text.WordWrap
                }
            }
            
            // Spacer
            Item { Layout.fillHeight: true }
            
            // Buttons
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                spacing: 8
                
                Repeater {
                    model: dialog.buttons
                    
                    delegate: Rectangle {
                        id: buttonRect
                        width: Math.max(80, buttonText.implicitWidth + 24)
                        height: 32
                        radius: 8
                        color: isDefault ? "#E85D04" : (buttonMA.containsMouse ? "#F0F0F0" : "#E8E8E8")
                        border.width: isDefault ? 0 : 1
                        border.color: "#CCCCCC"
                        
                        property bool isDefault: index === dialog.defaultButton
                        
                        Text {
                            id: buttonText
                            anchors.centerIn: parent
                            text: modelData
                            font.family: "Inter"
                            font.pixelSize: 13
                            font.weight: isDefault ? Font.DemiBold : Font.Normal
                            color: isDefault ? "white" : "#1A1A1A"
                        }
                        
                        MouseArea {
                            id: buttonMA
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: {
                                dialog.buttonClicked(index, modelData)
                                
                                // Standard button handling
                                if (modelData === "OK" || modelData === "Yes" || modelData === "Accept") {
                                    dialog.accepted()
                                } else if (modelData === "Cancel" || modelData === "No" || modelData === "Reject") {
                                    dialog.rejected()
                                }
                                
                                dialog.close()
                            }
                        }
                        
                        Behavior on color { ColorAnimation { duration: 100 } }
                    }
                }
            }
        }
    }
    
    // Keyboard handling
    Keys.onEscapePressed: {
        if (escapeButton >= 0 && escapeButton < buttons.length) {
            buttonClicked(escapeButton, buttons[escapeButton])
        }
        rejected()
        close()
    }
    
    Keys.onReturnPressed: {
        if (defaultButton >= 0 && defaultButton < buttons.length) {
            buttonClicked(defaultButton, buttons[defaultButton])
            if (buttons[defaultButton] === "OK" || buttons[defaultButton] === "Yes") {
                accepted()
            }
        }
        close()
    }
    
    // Helper functions
    function iconColor() {
        switch (iconType) {
            case "info": return "#3D5A80"     // Mission Blue
            case "warning": return "#D4A93E"  // Mars Gold
            case "error": return "#E85D04"    // Space Orange
            case "question": return "#3D5A80" // Mission Blue
            default: return "#666666"
        }
    }
    
    function iconSymbol() {
        switch (iconType) {
            case "info": return "i"
            case "warning": return "!"
            case "error": return "×"
            case "question": return "?"
            default: return ""
        }
    }
}
