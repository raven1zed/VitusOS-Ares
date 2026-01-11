/**
 * SystemTray.qml - macOS-style System Tray with Squircle containers
 * Aligned with Dock design language.
 * Now with LIVE StatusNotifier icons!
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
        property string iconId: ""
        
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
            color: parent.iconColor
            opacity: 0.85
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (iconId) {
                    systemTrayController.trayIconClicked(iconId)
                }
            }
            onPressed: bg.color = Qt.rgba(1, 1, 1, 0.6)
            onReleased: bg.color = Qt.rgba(1, 1, 1, 0.45)
        }
    }
    
    // Dynamic tray icons from StatusNotifier apps
    Repeater {
        model: systemTrayController.trayIcons
        
        delegate: TrayIcon {
            iconSource: {
                let iconName = modelData.iconName || "application-x-executable"
                return "image://icon/" + iconName
            }
            iconId: modelData.id || ""
        }
    }
    
    // Hardcoded system status icons (always visible)
    TrayIcon {
        iconSource: Qt.resolvedUrl("../../../assets/icons/battery.svg")
    }
    
    TrayIcon {
        iconSource: Qt.resolvedUrl("../../../assets/icons/volume.svg")
    }
    
    TrayIcon {
        iconSource: Qt.resolvedUrl("../../../assets/icons/network.svg")
    }
}
