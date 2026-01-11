/**
 * DropdownMenu.qml - Tidy macOS-style dropdown menu
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects

Popup {
    id: menuPopup
    
    padding: 6
    width: 220
    
    property var menuItems: []
    property int menuIndex: 0
    
    background: Rectangle {
        color: Qt.rgba(0.98, 0.98, 0.98, 0.98)
        radius: 12
        border.color: Qt.rgba(0, 0, 0, 0.1)
        border.width: 0.5
        
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            color: "#35000000"
            radius: 20
            samples: 33
            verticalOffset: 6
        }
    }
    
    contentItem: Column {
        id: menuCol
        spacing: 1
        width: parent.width
        
        Repeater {
            model: menuPopup.menuItems
            
            delegate: Button {
                id: btn
                width: menuCol.width
                flat: true
                height: 30
                
                property var itemData: modelData
                
                contentItem: Item {
                    anchors.fill: parent
                    
                    Text {
                        text: btn.itemData.label || "Item"
                        color: btn.hovered ? "white" : "#1A1A1A"
                        font.pixelSize: 13
                        font.family: "Inter"
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Text {
                        text: btn.itemData.shortcut || ""
                        color: btn.hovered ? "white" : "#888888"
                        font.pixelSize: 11
                        font.family: "Inter"
                        anchors.right: parent.right
                        anchors.rightMargin: 12
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                background: Rectangle {
                    color: btn.hovered ? "#D4622A" : "transparent"
                    radius: 8
                    anchors.fill: parent
                    anchors.margins: 1
                }
                
                onClicked: {
                    panelController.menuItemClicked(menuPopup.menuIndex, index)
                    menuPopup.close()
                }
            }
        }
    }
    
    function showAt(x, y, items, idx) {
        menuPopup.menuItems = items
        menuPopup.menuIndex = idx
        menuPopup.x = x
        menuPopup.y = y
        menuPopup.open()
    }
}
