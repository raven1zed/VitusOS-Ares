import QtQuick
import QtQuick.Controls

/**
 * BootSplash.qml - VitusOS Ares Boot Screen
 * 
 * This IS the AnimationEngine visual layer.
 * NOT Plymouth. NOT systemd output.
 * 
 * This component dominates the display from power-on to desktop,
 * showing the VitusOS | ARES logo with real systemd progress.
 */
Rectangle {
    id: bootSplash
    anchors.fill: parent
    color: "#000000" // Pure black background

    // Progress from AnimationEngine (0.0 to 1.0)
    property real progress: 0.0
    property bool isFirstBoot: false

    // VitusOS | ARES Logo - centered
    Row {
        id: logoRow
        anchors.centerIn: parent
        spacing: 10

        Text {
            id: vitusText
            text: "vitusOS"
            font.family: "Inter"
            font.pixelSize: 28
            font.weight: Font.Light
            color: "#FFFFFF"
        }

        Rectangle {
            width: 1
            height: 24
            color: "#808080" // Gray separator
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: aresText
            text: "ARES"
            font.family: "Inter"
            font.pixelSize: 28
            font.weight: Font.Medium // Medium weight, not bold, not italic
            color: "#FF6600" // Ares orange
        }
    }

    // Progress Bar - centered, balanced between logo and bottom
    // Position: halfway between logo bottom and screen bottom
    Rectangle {
        id: progressBar
        anchors.horizontalCenter: parent.horizontalCenter
        // Balanced position: logo center + half distance to bottom
        y: parent.height * 0.70 // ~70% down from top (balanced)
        width: 200
        height: 2 // Thin line
        color: "#FF6600" // Ares orange - no background track

        // Clip to show only progress portion
        clip: true
        
        Rectangle {
            id: progressFill
            x: 0
            y: 0
            width: progressBar.width * bootSplash.progress
            height: parent.height
            color: "#FF6600" // Same orange

            Behavior on width {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.OutCubic
                }
            }
        }
        
        // Background track (invisible until progress fills)
        Rectangle {
            x: progressBar.width * bootSplash.progress
            y: 0
            width: progressBar.width - (progressBar.width * bootSplash.progress)
            height: parent.height
            color: "#1A1A1A" // Very dark, nearly invisible track
        }
    }

    // Fade in animation when splash appears
    opacity: 0
    Component.onCompleted: {
        fadeIn.start()
    }

    NumberAnimation {
        id: fadeIn
        target: bootSplash
        property: "opacity"
        from: 0
        to: 1
        duration: 500
        easing.type: Easing.OutCubic
    }

    // Fade out function for transition to desktop
    function fadeToDesktop() {
        fadeOut.start()
    }

    NumberAnimation {
        id: fadeOut
        target: bootSplash
        property: "opacity"
        from: 1
        to: 0
        duration: 400
        easing.type: Easing.InCubic
        onFinished: bootSplash.visible = false
    }

    // Simulate progress for testing (remove in production)
    Timer {
        id: progressTimer
        interval: 500
        running: false // Set to true for testing
        repeat: true
        onTriggered: {
            if (bootSplash.progress < 1.0) {
                bootSplash.progress += 0.1
            } else {
                progressTimer.stop()
                fadeToDesktop()
            }
        }
    }
}
