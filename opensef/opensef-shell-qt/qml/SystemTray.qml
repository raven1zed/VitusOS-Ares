/**
 * SystemTray.qml - macOS-style System Tray with Squircle containers
 * Aligned with Dock design language.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects

Row {
    id: systemTray
    spacing: 8 
    height: 24
    
    // Component for a tray item in a squircle
    component TrayIcon : Item {
        property alias iconSource: trayImg.source
        property color iconColor: "#1A1A1A"
        
        width: 24
        height: 24
        
        Rectangle {
            id: bg
            anchors.fill: parent
            radius: 6
            color: Qt.rgba(1, 1, 1, 0.45)
            border.width: 0.5
            border.color: Qt.rgba(0, 0, 0, 0.05)
        }
        
        Image {
            id: trayImg
            anchors.fill: parent
            anchors.margins: 5
            sourceSize: Qt.size(32, 32)
            smooth: true
            antialiasing: true
            visible: false // Hidden to use with ColorOverlay
        }

        ColorOverlay {
            anchors.fill: trayImg
            source: trayImg
            color: iconColor
            opacity: 0.85
        }
    }

    // Network
    TrayIcon {
        iconSource: Qt.resolvedUrl("../../../assets/icons/network.svg")
    }
    
    // Volume
    TrayIcon {
        iconSource: Qt.resolvedUrl("../../../assets/icons/volume.svg")
    }

    // Battery
    TrayIcon {
        iconSource: Qt.resolvedUrl("../../../assets/icons/battery.svg")
    }

    // Dynamic app icons - from framework
    Repeater {
        model: systemTrayController.trayIcons
        delegate: TrayIcon {
            iconSource: modelData.iconName ? "image://icon/" + modelData.iconName : ""
        }
    }
}
