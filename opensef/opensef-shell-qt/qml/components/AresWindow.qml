/**
 * AresWindow.qml - Universal openSEF Window Base
 *
 * Enforces the 20px 4-corner rounding standard using high-fidelity OpacityMask.
 * Use this as the root for ALL VitusOS Ares applications.
 */

import QtQuick 2.15
import QtQuick.Window 2.15
import Qt5Compat.GraphicalEffects

Window {
    id: aresWindow
    
    // Default Ares Settings
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"
    visible: true
    
    // Properties
    property string fontFamily: "Inter"
    property alias mainColor: mainRect.color
    property alias cornerRadius: maskRect.radius
    property bool showDecorations: true
    property string aresTitle: ""
    
    // Mask for consistent 4-corner rounding
    Item {
        id: container
        anchors.fill: parent
        
        Rectangle {
            id: maskRect
            anchors.fill: parent
            radius: 20 // Ares Standard
            color: "black"
            visible: false
        }
        
        Rectangle {
            id: mainRect
            anchors.fill: parent
            color: "#FAFAF9"
            radius: maskRect.radius
            
            // High-fidelity clipping
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: maskRect
            }
            
            // Content Area
            Item {
                id: windowContent
                anchors.fill: parent
            }
        }
    }
    
    // Geometry sync for Dock Autohide
    onXChanged: syncGeometry()
    onYChanged: syncGeometry()
    onWidthChanged: syncGeometry()
    onHeightChanged: syncGeometry()
    
    function syncGeometry() {
        if (typeof panelController !== "undefined") {
            panelController.reportWindowGeometry(window.aresTitle.toLowerCase(), x, y, width, height)
        }
    }
    
    // Forward children to windowContent
    default property alias data: windowContent.data
}
