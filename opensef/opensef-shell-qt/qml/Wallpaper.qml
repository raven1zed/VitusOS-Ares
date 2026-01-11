/**
 * Wallpaper.qml - Desktop Wallpaper
 * 
 * Renders the Mars-themed wallpaper as the desktop background
 */

import QtQuick 2.15

Rectangle {
    id: wallpaper
    
    // 1. Fallback Gradient (Always Visible - Bottom Layer)
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1a0f0a" }  // Dark Mars sky
            GradientStop { position: 0.4; color: "#3d1f16" }  // Horizon
            GradientStop { position: 0.6; color: "#8b4a2a" }  // Mars orange
            GradientStop { position: 1.0; color: "#c4785c" }  // Mars sand
        }
    }

    // 2. Wallpaper Image (Top Layer)
    Image {
        id: wallpaperImage
        anchors.fill: parent
        source: "file:///mnt/c/Users/hp/Documents/VitusOS-Ares/resources/wallpapers/mars_default.jpg"
        fillMode: Image.PreserveAspectCrop
        
        // Only visible if loaded successfully
        visible: status === Image.Ready
        
        onStatusChanged: {
            if (status === Image.Error) {
                console.log("[Wallpaper] Image load error: " + source)
            }
        }
    }
}
