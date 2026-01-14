/**
 * AresMenu.qml - Universal openSEF Dropdown/Popup Base
 *
 * Enforces the 20px 4-corner rounding standard for menus and popups.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Menu {
    id: aresMenu
    
    background: Item {
        implicitWidth: 200
        implicitHeight: 40
        
        Rectangle {
            id: maskRect
            anchors.fill: parent
            radius: 20
            color: "black"
            visible: false
        }
        
        Rectangle {
            id: bgRect
            anchors.fill: parent
            color: "#FAFAF9" // Warm Ceramic
            radius: 20
            border.color: "#E0E0E0"
            border.width: 1
            
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: maskRect
            }
        }
    }
}
