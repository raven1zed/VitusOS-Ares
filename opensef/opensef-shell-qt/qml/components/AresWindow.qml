import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

/**
 * AresWindow.qml - Universal OpenSEF Window Base
 * Implements "The Ares Corner" (20px radius) with high-fidelity clipping.
 */
Item {
    id: aresWindow
    
    // DEFAULT property so children automatically go into content
    default property alias content: contentItem.data
    property string title: ""
    property bool showDecorations: true
    property color backgroundColor: "#FFFFFF"
    property real cornerRadius: 20
    property var targetWindow: null

    // Background with all 4 rounded corners
    Rectangle {
        id: background
        anchors.fill: parent
        radius: aresWindow.cornerRadius
        color: aresWindow.backgroundColor
    }

    // WindowDecorations (OUTSIDE any layer.effect to avoid glitches)
    WindowDecorations {
        id: decorations
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        visible: aresWindow.showDecorations
        windowTitle: aresWindow.title
        targetWindow: aresWindow.targetWindow
        z: 1  // Above content
    }

    // Content area with bottom corner masking
    Item {
        id: contentContainer
        anchors.top: decorations.visible ? decorations.bottom : parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        
        // Mask for bottom rounded corners only
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: contentContainer.width
                height: contentContainer.height
                radius: aresWindow.cornerRadius
                // Square top corners (they're covered by decorations)
                Rectangle {
                    width: parent.width
                    height: parent.radius
                    color: "white"
                }
            }
        }

        // Content goes here
        Item {
            id: contentItem
            anchors.fill: parent
        }
    }

    // Shadow
    DropShadow {
        anchors.fill: background
        horizontalOffset: 0
        verticalOffset: 12
        radius: 24
        samples: 32
        color: "#30000000"
        source: background
        z: -1
    }
}
