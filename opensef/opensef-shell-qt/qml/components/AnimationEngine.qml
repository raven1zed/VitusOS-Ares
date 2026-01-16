pragma Singleton
import QtQuick 2.15

/**
 * AnimationEngine - The heart of VitusOS Ares fluid interactions.
 * 
 * Provides standardized timings and easing curves to ensure
 * consistency across the entire shell and native applications.
 */
QtObject {
    // Standard Ares Timings (in milliseconds)
    readonly property int instant: 100    // Micro-interactions (hover, click)
    readonly property int quick: 200      // Popovers, tooltips
    readonly property int standard: 300   // Window movements, drawers
    readonly property int slow: 500       // Page transitions, workspace switches
    readonly property int bounce: 600     // Bouncy emphasized animations

    // Easing Curves
    readonly property int outCubic: Easing.OutCubic
    readonly property int inOutCubic: Easing.InOutCubic
    readonly property int outBack: Easing.OutBack
    readonly property int outQuint: Easing.OutQuint
    
    // Animation Factory (convenience)
    function create(target, property, value, duration = standard, easing = outCubic) {
        // Implementation for dynamic animation creation if needed
    }
}
