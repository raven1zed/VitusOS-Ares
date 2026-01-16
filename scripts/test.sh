#!/bin/bash
# test.sh - Verify openSEF Desktop functionality
# Runs automated tests and verification checks

set -e

echo "╔════════════════════════════════════════════╗"
echo "║  VitusOS Ares - Verification Tests        ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# Test counter
PASSED=0
FAILED=0

# Test function
test_item() {
    local category=$1
    local name=$2
    local command=$3
    
    echo -n "  [$category] $name... "
    
    if eval "$command" &>/dev/null; then
        echo "✅"
        ((PASSED++))
    else
        echo "❌"
        ((FAILED++))
    fi
}

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "📦 Component Build Status"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "Build" "Framework" "[ -f opensef/opensef-framework/build/libopensef.so ]"
test_item "Build" "Compositor" "[ -f opensef/opensef-compositor/build/opensef-compositor ]"
test_item "Build" "GNUstep" "[ -f opensef/opensef-gnustep/build/libgnustep-appkit.so ]"
test_item "Build" "Shell" "[ -f opensef/opensef-shell-qt/build/opensef-shell-qt ]"
test_item "Build" "Filer" "[ -f opensef/apps/osf-filer-native/build/osf-filer-native ]"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "📝 Source Code Checks"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "Code" "Vision Document" "[ -f vitusos_complete_vision.md ]"
test_item "Code" "Flake Config" "[ -f flake.nix ]"
test_item "Code" "Panel QML" "[ -f opensef/opensef-shell-qt/qml/AresPanel.qml ]"
test_item "Code" "Dock QML" "[ -f opensef/opensef-shell-qt/qml/Dock.qml ]"
test_item "Code" "SystemTray QML" "[ -f opensef/opensef-shell-qt/qml/SystemTray.qml ]"
test_item "Code" "SystemTray Dropdown" "[ -f opensef/opensef-shell-qt/qml/components/SystemTrayDropdown.qml ]"
test_item "Code" "Filer QML" "[ -f opensef/apps/osf-filer-native/Filer.qml ]"
test_item "Code" "EventBus Header" "[ -f opensef/opensef-framework/include/OSFEventBus.h ]"
test_item "Code" "WindowManager Header" "[ -f opensef/opensef-framework/include/OSFWindowManager.h ]"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo  "🎨 Design System Compliance"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Check for Inter font usage
INTER_COUNT=$(grep -r "font.family.*Inter" opensef/opensef-shell-qt/qml/ 2>/dev/null | wc -l)
test_item "Design" "Inter Font Usage (${INTER_COUNT} files)" "[ $INTER_COUNT -gt 20 ]"

# Check for Ares colors
test_item "Design" "Space Orange Color" "grep -r '#E85D04\\|#E57C3A' opensef/ &>/dev/null"
test_item "Design" "Mars Gold Color" "grep -r  '#D4A93E' opensef/ &>/dev/null"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "🏗️  Architecture Compliance"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Check for OSFDesktop usage
test_item "Arch" "OSFDesktop::shared() usage" "grep -r 'OSFDesktop::shared()' opensef/opensef-shell-qt/src/ &>/dev/null"

# Check for EventBus usage
test_item "Arch" "EventBus subscriptions" "grep -r 'eventBus()->subscribe' opensef/opensef-shell-qt/src/ &>/dev/null"

# Check for GNUstep components
test_item "Arch" "NSButton implementation" "[ -f opensef/opensef-gnustep/include/opensef/NSButton.h ]"
test_item "Arch" "NSView implementation" "[ -f opensef/opensef-gnustep/include/opensef/NSView.h ]"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "📊 Test Summary"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "  ✅ Passed: $PASSED"
echo "  ❌ Failed: $FAILED"
echo "  📈 Total:  $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "🎉 All tests passed! openSEF is ready to run."
    echo ""
    echo "Next: ./scripts/run.sh"
    exit 0
else
    echo "⚠️  Some tests failed. Review the output above."
    echo ""
    echo "If builds are missing: ./scripts/build.sh"
    exit 1
fi
