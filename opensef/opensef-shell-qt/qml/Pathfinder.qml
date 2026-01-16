import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Pathfinder.qml - Universal Command Center UI
 * 
 * Activated via Cmd+Space (or Meta+Space on Linux).
 * Searches: apps, files, clipboard, NixOS packages, web.
 */
Rectangle {
    id: pathfinder
    width: 600
    height: Math.min(400, searchField.height + resultsContainer.height + 32)
    radius: 12
    color: "#1A1A1A" // Dark background
    border.color: "#333333"
    border.width: 1
    opacity: 0
    visible: false

    property bool isVisible: false

    // Show/Hide with animation
    function show() {
        visible = true
        isVisible = true
        showAnimation.start()
        searchField.forceActiveFocus()
    }

    function hide() {
        hideAnimation.start()
    }

    NumberAnimation {
        id: showAnimation
        target: pathfinder
        property: "opacity"
        from: 0
        to: 1
        duration: 150
        easing.type: Easing.OutCubic
    }

    NumberAnimation {
        id: hideAnimation
        target: pathfinder
        property: "opacity"
        from: 1
        to: 0
        duration: 100
        easing.type: Easing.InCubic
        onFinished: {
            pathfinder.visible = false
            pathfinder.isVisible = false
            searchField.text = ""
        }
    }

    // Close on Escape
    Keys.onEscapePressed: hide()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Search Input
        TextField {
            id: searchField
            Layout.fillWidth: true
            placeholderText: "Search apps, files, packages..."
            font.pixelSize: 18
            font.family: "Inter"
            color: "#FFFFFF"
            placeholderTextColor: "#666666"

            background: Rectangle {
                radius: 8
                color: "#2A2A2A"
                border.color: searchField.activeFocus ? "#FF6600" : "#404040"
                border.width: 1
            }

            onTextChanged: {
                // TODO: Call Pathfinder.search(text)
                searchTimer.restart()
            }

            Keys.onDownPressed: resultsList.currentIndex = 0
            Keys.onReturnPressed: {
                if (resultsList.currentIndex >= 0) {
                    resultsList.currentItem.execute()
                }
            }
        }

        // Search Timer (debounce)
        Timer {
            id: searchTimer
            interval: 150
            onTriggered: {
                // TODO: pathfinderController.search(searchField.text)
            }
        }

        // Results Container
        Rectangle {
            id: resultsContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
            visible: resultsList.count > 0

            ListView {
                id: resultsList
                anchors.fill: parent
                clip: true
                spacing: 4

                // Placeholder model - will be connected to PathfinderController
                model: ListModel {
                    id: resultsModel
                }

                delegate: Rectangle {
                    width: resultsList.width
                    height: 48
                    radius: 6
                    color: ListView.isCurrentItem ? "#FF660033" : (mouseArea.containsMouse ? "#2A2A2A" : "transparent")

                    property string resultId: model.id || ""
                    property string resultType: model.type || ""

                    function execute() {
                        console.log("Execute:", resultId, resultType)
                        // TODO: pathfinderController.execute(resultId, resultType)
                        pathfinder.hide()
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12
                        spacing: 12

                        // Icon placeholder
                        Rectangle {
                            width: 32
                            height: 32
                            radius: 6
                            color: "#333333"

                            Text {
                                anchors.centerIn: parent
                                text: {
                                    switch (model.type) {
                                        case "Application": return "📱"
                                        case "File": return "📄"
                                        case "NixPackage": return "📦"
                                        case "WebSearch": return "🌐"
                                        case "SystemAction": return "⚙️"
                                        default: return "🔍"
                                    }
                                }
                                font.pixelSize: 16
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2

                            Text {
                                text: model.title || ""
                                font.pixelSize: 14
                                font.family: "Inter"
                                font.weight: Font.Medium
                                color: "#FFFFFF"
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Text {
                                text: model.subtitle || ""
                                font.pixelSize: 11
                                font.family: "Inter"
                                color: "#888888"
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }

                        // Type badge
                        Rectangle {
                            width: typeLabel.width + 12
                            height: 20
                            radius: 4
                            color: {
                                switch (model.type) {
                                    case "Application": return "#2D5A27"
                                    case "NixPackage": return "#5A4A27"
                                    case "SystemAction": return "#5A2727"
                                    default: return "#333333"
                                }
                            }
                            visible: model.type === "NixPackage" || model.type === "SystemAction"

                            Text {
                                id: typeLabel
                                anchors.centerIn: parent
                                text: model.type === "NixPackage" ? "Install" : "Action"
                                font.pixelSize: 10
                                font.family: "Inter"
                                color: "#CCCCCC"
                            }
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            resultsList.currentIndex = index
                            execute()
                        }
                    }
                }

                // Keyboard navigation
                Keys.onUpPressed: {
                    if (currentIndex > 0) currentIndex--
                }
                Keys.onDownPressed: {
                    if (currentIndex < count - 1) currentIndex++
                }
                Keys.onReturnPressed: {
                    if (currentItem) currentItem.execute()
                }
            }
        }

        // Empty state
        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            visible: resultsList.count === 0 && searchField.text.length > 0
            text: "No results found"
            font.pixelSize: 14
            font.family: "Inter"
            color: "#666666"
            horizontalAlignment: Text.AlignHCenter
        }

        // Hint text
        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            visible: searchField.text.length === 0
            text: "Search apps, files, or type to install packages"
            font.pixelSize: 12
            font.family: "Inter"
            color: "#555555"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // Drop shadow effect
    layer.enabled: true
    layer.effect: Item {
        // Shadow would go here - simplified for now
    }
}
