import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects 1.0

/**
 * AresWindow.qml - Universal OpenSEF Window Base
 * Implements "The Ares Corner" (20px radius) with high-fidelity clipping.
 */
Item {
    id: aresWindow
    
    property alias content: contentItem.data
    property string title: ""
    property bool showDecorations: true
    property color backgroundColor: "#FFFFFF"
    property real cornerRadius: 20

    // Main frame with rounded corners
    Rectangle {
        id: body
        anchors.fill: parent
        radius: aresWindow.cornerRadius
        color: aresWindow.backgroundColor
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: body.width
                height: body.height
                radius: body.radius
            }
        }

        // Titlebar / Decorations
        WindowDecorations {
            id: decorations
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            visible: aresWindow.showDecorations
            windowTitle: aresWindow.title
        }

        // Content Container
        Item {
            id: contentItem
            anchors.top: decorations.visible ? decorations.bottom : parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }

    // Shadow
    DropShadow {
        anchors.fill: body
        horizontalOffset: 0
        verticalOffset: 10
        radius: 30
        samples: 60
        color: "#40000000"
        source: body
        z: -1
    }
}
