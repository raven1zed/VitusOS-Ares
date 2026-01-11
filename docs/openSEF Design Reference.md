# openSEF Design Reference

**VitusOS Ares Desktop Environment**

*Design Specification & Style Guide*

---

# 1. Design Philosophy

VitusOS Ares combines three core aesthetics:

1.  **OS1 (Her, 2013)**: Warmth, human-centric, "technology that recedes".
2.  **macOS Aqua**: Vitality, polish, fluid animations, "lickable".
3.  **Ares (The Martian)**: Determination, engineering precision, "Science the sh*t out of this".

### The "Ares" Aesthetic
- **Warmth**: Avoid sterile cold blues. Use colors of earth, skin, and wood.
- **Precision**: Pixel-perfect alignment, consistent spacing.
- **Depth**: Subtle shadows and translucent "glass" materials, but mostly opaque "ceramic" surfaces.

> **Key Difference from macOS**: macOS is "Glass and Aluminium". Ares is "Ceramic and Warm Light".

---

# 2. Implemented UI Components (Phase 4)

## The Panel (`OSFPanel`)
- **Height**: 28px (Fixed)
- **Position**: Top of screen
- **Color**: `LunarGray` (#F5F5F5) at 95% opacity
- **Shadow**: `0 1px 12px` soft shadow
- **Layout**:
  - **Left**: [Orange Accent Box 20px+] + [Global Menu]
  - **Center**: [Active Window Title] (Bold)
  - **Right**: [Clock] (HH:mm)

## The Dock (`OSFDock`)
- **Height**: 64px (Fixed per item)
- **Position**: Bottom Center (Floating)
- **Corner Radius**: 16px (Continuous Curve)
- **Color**: `DockBackground` (#B3FFFFFF - Translucent White)
- **Interaction**:
  - Hover: slight scale up
  - Click: Bounce animation (Physics based)
  - Active: Small dot indicator below icon

---

# 3. Ares Theme Palette

Defined in `OSFAresTheme.h`.

### Primary Colors
| Name | Hex | Usage |
|------|-----|-------|
| **Space Orange** | `#E85D04` | Primary Accent, Close Button, Focus |
| **Mars Gold** | `#D4A93E` | Secondary Accent, Minimize Button |
| **Mission Blue** | `#3D5A80` | Tertiary Accent, Maximize Button |

### Surfaces
| Name | Hex | Usage |
|------|-----|-------|
| **Star White** | `#FFFFFF` | Window Backgrounds, Input Fields |
| **Lunar Gray** | `#F5F5F5` | Panel Background, Title Bars |
| **Deep Space** | `#1A1A1A` | Dark Mode Backgrounds |

### Semantic Colors
| Name | Hex | Usage |
|------|-----|-------|
| **Text Primary** | `#1A1A1A` | High contrast text |
| **Text Secondary** | `#555555` | Labels, subtitles |
| **Glass** | `#F2F5F5F5`| 95% Opacity White (Panel) |

---

# 4. Window System Specification

### Traffic Lights
Unlike macOS which groups them tight left, Ares spaces them slightly more for "Touch/Mouse friendly" precision.
- **Size**: 12px diameter
- **Spacing**: 8px gap
- **Colors**: Orange (Close), Gold (Minimize), Blue (Maximize) - *Note: Distinct from macOS Red/Yellow/Green to fit Mars theme.*

### Title Bar
- **Height**: 28px
- **Text**: Centered, Bold
- **Integration**: Unified with toolbar (NSWindowStyleMaskUnifiedTitleAndToolbar equivalent).

---

# 5. Iconography
- **Style**: "Superellipse" shapes (Squircle).
- **Shadow**: Meaningful drop shadow (light coming from top-left).
- **Theme**: Currently using "WhiteSur" icons as placeholder/reference for Aqua feel.

---

# 6. Animation Guidelines (Core Animation Style)
- **Ease Out**: `cubic-bezier(0, 0, 0.2, 1)` - Most UI movements.
- **Spring**: Used for Dock scaling and Window opening.
- **Durations**:
  - Hover: 150ms
  - Window Open/Close: 350ms (Spring)
  - Workspace Switch: 500ms

---

*This document serves as the "Human Interface Guidelines" (HIG) for VitusOS Ares.*
