import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects 1.0

/**
 * AresMenu.qml - Universal OpenSEF Dropdown/Popup Base
 * Implements "The Ares Corner" (20px radius) and warm ceramic aesthetic.
 */
Menu {
    id: aresMenu
    
    background: Item {
        implicitWidth: 200
        implicitHeight: 40
        
        Rectangle {
            id: bgRect
            anchors.fill: parent
            radius: 20
            color: "#FAFAF9" // Warm Ceramic
            border.color: "#E5E5E4"
            border.width: 1
            
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: bgRect.width
                    height: bgRect.height
                    radius: bgRect.radius
                }
            }
        }
        
        DropShadow {
            anchors.fill: bgRect
            horizontalOffset: 0
            verticalOffset: 5
            radius: 15
            samples: 30
            color: "#30000000"
            source: bgRect
            z: -1
        }
    }
    
    delegate: MenuItem {
        id: menuItem
        implicitWidth: 200
        implicitHeight: 36
        
        contentItem: Text {
            text: menuItem.text
            font.family: "Inter"
            font.pixelSize: 13
            color: menuItem.highlighted ? "#FFFFFF" : "#1A1A1A"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            leftPadding: 20
        }
        
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 36
            opacity: menuItem.highlighted ? 1 : 0
            color: "#D4A93E" // Mars Gold (Selection color)
            radius: 10
            anchors.margins: 4
        }
    }
}
