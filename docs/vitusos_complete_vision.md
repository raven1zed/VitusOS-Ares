# VitusOS Ares - Complete Vision Document

**The Crystal Clear Blueprint for openSEF Desktop**

---

## Table of Contents

1. [Core Philosophy](#core-philosophy)
2. [What openSEF Is](#what-opensef-is)
3. [Architecture Overview](#architecture-overview)
4. [System Hierarchy](#system-hierarchy)
5. [Technical Stack](#technical-stack)
6. [Desktop Components](#desktop-components)
7. [Window Management](#window-management)
8. [Multitask View](#multitask-view)
9. [Application Framework](#application-framework)
10. [System Integration](#system-integration)
11. [Design System](#design-system)
12. [Release Channels](#release-channels)
13. [Development Phases](#development-phases)

---

## Core Philosophy

### The Fundamental Principle

**VitusOS Ares is NOT a traditional Linux Desktop Environment.**

It is a **complete operating system experience** built on Linux, the same way macOS is built on Darwin/XNU.

### What This Means

**❌ NOT Like Traditional Linux DEs (GNOME/KDE/XFCE):**

- Fragmented components communicating via D-Bus
- Each app loads its own theme
- Inconsistent UI/UX across applications
- Separate window manager, compositor, panel, dock
- Visible systemd logs during boot
- User-facing configuration files scattered everywhere

**✅ Like macOS/OS X:**

- **ONE unified framework** controlling everything
- **ONE design language** from boot to shutdown
- **Complete integration** - all components speak the same language
- **Hidden internals** - users never see systemd, kernel logs, or system plumbing
- **Seamless experience** - it just works as one cohesive system

### The Guiding Vision

> "A desktop environment should feel like ONE system, not a collection of apps"

From the moment you power on until shutdown, every component speaks the same language through openSEF. No fragmentation, no protocols exposed to users - just a seamless experience.

**Inspired by:**

- macOS Cocoa/AppKit architecture
- NeXTSTEP unified framework approach
- OS1 from "Her" (2013) - technology that recedes
- The Martian aesthetic - warm, determined, human

---

## What openSEF Is

openSEF (Open Seamless Environment Framework) is **THREE things simultaneously:**

### 1. Desktop Environment

The complete user experience from boot to shutdown:

- Boot animation (no systemd logs)
- Login screen
- Desktop shell (panel, dock, wallpaper)
- Window management and tiling
- Multitask view
- System services
- Shutdown animation

### 2. API Layer

The programming interface that everything uses:

```cpp
// Everything goes through this central API
OSFDesktop::shared()

// Core APIs:
- EventBus (publish/subscribe for all events)
- StateManager (central state management)
- WindowManager (window lifecycle and positioning)
- ClipboardManager (Universal Clipboard)
- AnimationEngine (smooth transitions everywhere)
- ThemeEngine (consistent visual design)
```

### 3. Application Framework

The foundation for building native applications:

- GNUstep AppKit (forked and converted to C++)
- Widget library (buttons, windows, text fields, etc.)
- Native app behavior (menus, toolbars, preferences)
- Seamless integration with desktop features

**Critical Understanding:**
openSEF is not "a compositor + a shell + some apps". It is **ONE unified framework** that happens to include a compositor, shell, and app toolkit - but they're all parts of the same system, not separate components.

---

## Architecture Overview

### The Unified Model

```
┌─────────────────────────────────────────────────────────────┐
│                    VitusOS Ares Desktop                     │
│                  (Complete User Experience)                 │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌───────────────────────────────────────────────────────┐  │
│  │            openSEF Framework (C++)                    │  │
│  │                                                       │  │
│  │  OSFDesktop::shared() - Central API                   │  │
│  │  ├─ EventBus (pub/sub for all events)                 │  │
│  │  ├─ StateManager (centralized state)                  │  │
│  │  ├─ WindowManager (window lifecycle)                  │  │
│  │  ├─ ClipboardManager (Universal Clipboard)            │  │
│  │  ├─ AnimationEngine (smooth transitions)              │  │
│  │  ├─ ThemeEngine (Ares design system)                  │  │
│  │  └─ TilingManager (intelligent window tiling)         │  │
│  │                                                       │  │
│  └───────────────────────────────────────────────────────┘  │
│                            ↕                                │
│  ┌───────────────────────────────────────────────────────┐  │
│  │              Desktop Components                       │  │
│  │                                                       │  │
│  │  Compositor (C + wlroots + Vulkan)                    │  │
│  │  ├─ Wayland protocol server                           │  │
│  │  ├─ Scene graph rendering                             │  │
│  │  ├─ XDG shell for client windows                      │  │
│  │  └─ Layer shell for panels/dock                       │  │
│  │                                                       │  │
│  │  Shell (Qt Quick + QML + Vulkan via QRhi)             │  │
│  │  ├─ Panel (global menu bar, system tray)              │  │
│  │  ├─ Dock (app launcher with intelligence)             │  │
│  │  ├─ Wallpaper (dynamic background)                    │  │
│  │  └─ Multitask View (virtual desktop overview)         │  │
│  │                                                       │  │
│  │  Native Apps (GNUstep AppKit C++ fork)                │  │
│  │  ├─ Filer (file manager, always running)              │  │
│  │  ├─ Terminow (terminal emulator)                      │  │
│  │  ├─ Settings (system preferences)                     │  │
│  │  └─ SeaDrop (AirDrop-like file sharing)               │  │
│  │                                                       │  │
│  └───────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Key Architectural Principles

1. **Single Source of Truth**: `OSFDesktop::shared()` is the central point for all system state
2. **Event-Driven**: Everything communicates via EventBus, not D-Bus spaghetti
3. **Shared Resources**: One theme engine, one animation system, one state manager
4. **Internal APIs**: Components use C++ APIs, not Linux protocols (in up1)
5. **Unified Rendering**: Vulkan pipeline via QRhi for consistent GPU acceleration

---

## System Hierarchy

### The Power Structure

```
Hardware (Physical Machine)
    ↓
Kernel (Linux)
    ↓
systemd (Init System) ← TAMED AND HIDDEN
    ↓
openSEF (DOMINANT LAYER) ← CONTROLS EVERYTHING USERS SEE
    ↓
Desktop Experience (What users interact with)
```

### Critical Understanding: openSEF Dominates systemd

**systemd's Role:**

- Still runs as init system (PID 1)
- Manages system services in the background
- Handles hardware initialization
- **BUT**: Completely hidden from user view

**openSEF's Control:**

- Wraps systemd completely
- Shows custom boot animation (no systemd logs visible)
- Manages user session from login to logout
- Controls all visible system behavior
- Makes systemd invisible, like macOS hides launchd

**User Never Sees:**

- "Starting service X..."
- Systemd unit files
- Journal logs during boot
- Any systemd internals

**User Always Sees:**

- Smooth boot animation (Ares themed)
- Clean login screen
- Unified desktop experience
- Smooth shutdown animation

### Why This Matters

Traditional Linux DEs **expose** systemd to users. VitusOS Ares **completely wraps** it.

```
Traditional Linux:
Boot → [Systemd Logs] → Login → Desktop
      ↑ Users see this ugliness

VitusOS Ares:
Boot → [Ares Animation] → Login → Desktop
      ↑ Smooth, unified, beautiful
      ↓ systemd runs underneath, completely hidden
```

---

## Technical Stack

### Component Breakdown

| Component                 | Technology                             | Purpose                      | Why This Choice                                   |
| ------------------------- | -------------------------------------- | ---------------------------- | ------------------------------------------------- |
| **Framework Core**        | C++                                    | openSEF unified API          | Performance + OOP for Cocoa-like design           |
| **Compositor**            | C + wlroots 0.19 + Vulkan              | Wayland protocol server      | wlroots uses C99 syntax, Vulkan for GPU rendering |
| **Shell Rendering**       | Qt Quick + QML + Vulkan                | Panel, Dock, UI elements     | 600+ FPS GPU acceleration via QRhi                |
| **Rendering Abstraction** | QRhi (Qt Rendering Hardware Interface) | Cross-platform GPU rendering | Vulkan/Metal/D3D abstraction layer                |
| **Widget Library**        | GNUstep AppKit (C++ fork)              | Native app widgets           | macOS-like widget behavior and API                |
| **IPC**                   | C++ EventBus (internal)                | Component communication      | Unified pub/sub, no D-Bus fragmentation           |
| **Animation**             | C++ Animation Engine                   | Smooth transitions           | Consistent 60+ FPS animations everywhere          |
| **State Management**      | C++ StateManager                       | Centralized state            | Single source of truth for all components         |

### Why This Specific Stack?

**wlroots in C:**

- wlroots uses C99-only syntax (`[static 4]`) that C++ compilers reject
- Must be pure C for compatibility
- Same approach as labwc, sway, other production compositors

**Qt Quick for Shell:**

- GPU-accelerated rendering (600+ FPS capable)
- QML declarative UI (rapid iteration)
- QRhi abstraction (Vulkan/Metal/D3D support)
- Modern rendering pipeline

**GNUstep AppKit Fork:**

- Proven macOS-like widget framework
- Converting from Objective-C to C++ for openSEF integration
- Native app behavior (menus, toolbars, preferences)
- Mature codebase to build upon

**C++ for Framework:**

- Object-oriented design (like Cocoa)
- Performance (native compiled)
- Direct hardware access
- Modern language features (RAII, templates, smart pointers)

---

## Desktop Components

### Typography

**System Font:**

- **Primary**: Inter (all UI elements)
- **Monospace**: (Terminal only - not Inter)

**Font Weights:**

- Regular (400): Body text, labels
- Medium (500): Buttons, UI elements
- Semibold (600): Headings, emphasis
- Bold (700): Strong emphasis

**Why Inter:**

- Modern, clean sans-serif
- Excellent screen readability
- Professional appearance
- Consistent across all UI

### Panel (Global Menu Bar)

**Location**: Top of screen, 28px height

**Components (Left to Right):**

1. **Orange Box Button** (Left Corner)
   
   - Simple orange square icon
   - NO text label (not "Activities" - that's old Linux DE paradigm)
   - Click to open Cockpit View
   - Keyboard shortcut alternative available
   - Clean, iconic design

2. **Application Menu** (Dynamic)
   
   - Shows active application name
   - Displays application-specific menus (File, Edit, View, etc.)
   - Works for ALL apps:
     - Native openSEF apps (direct integration)
     - GTK apps (via DBusMenu protocol)
     - Qt apps (via DBusMenu protocol)  
     - Electron apps (via DBusMenu protocol)
   - Menus extracted and displayed in global bar (macOS style)

3. **System Tray** (Right Side)
   
   - Click to reveal dropdown menu
   - Shows background apps and system services:
     - Network status (WiFi, Ethernet)
     - Battery level (laptops)
     - Volume control
     - Bluetooth status
     - Other system indicators
   - Clean separation: active apps in dock, background services in tray

4. **Clock** (Far Right)
   
   - Current time display
   - Click for calendar dropdown

**Visual Design:**

- Ares color scheme (Lunar Gray background #F5F5F5)
- Smooth animations for menu transitions
- Consistent typography (system font)
- Subtle shadows for depth

**Technical Implementation:**

- Qt Quick + QML rendering
- Wayland layer shell protocol (always on top)
- Connects to OSFDesktop::shared() for app state
- EventBus subscription for menu updates

### Dock (Application Launcher)

**Location**: Bottom of screen, 64px height, 16px corner radius

**Behavior - Self-Intelligence:**

1. **Auto-Hide Logic**
   
   - Detects window overlap with dock area
   - Auto-hides when window maximizes
   - Slides down smoothly with animation
   - Returns when mouse approaches bottom edge
   - No manual configuration needed - it just knows

2. **App Icons**
   
   - Running apps show with dot indicator below icon
   - **Filer**: ALWAYS shows dot (always running, like Finder)
   - Click icon to launch or focus application
   - Right-click for context menu (Quit, Keep in Dock, etc.)

3. **Visual Feedback**
   
   - Hover: subtle scale animation (1.0 → 1.1)
   - Active app: dot indicator + subtle glow
   - Launching: bounce animation
   - Smooth icon reordering (drag to rearrange)

**Technical Implementation:**

- Qt Quick + QML + Vulkan rendering
- Wayland layer shell protocol
- Monitors window positions via WindowManager
- EventBus for app launch/quit events
- Animation engine for all transitions

### Wallpaper System

**Features:**

- Dynamic wallpaper support (time-of-day variants)
- Smooth transitions between wallpapers
- Per-desktop wallpaper support
- Efficient rendering (no redundant redraws)

**Technical:**

- Rendered as compositor background layer
- Vulkan-accelerated for smooth transitions
- Cached to minimize GPU usage

---

## Window Management

### Custom Window Decorations

**Why Custom Decorations:**

- Consistent visual design across all windows
- Better tiling integration
- openSEF controls the chrome, not the app
- Like macOS - OS controls window frame

**Decoration Elements:**

1. **Title Bar**
   
   - Height: 28px
   - Background: Lunar Gray (#F5F5F5)
   - Title: Center-aligned (or left, per design choice)
   - Smooth resize/drag interactions

2. **Traffic Light Buttons** (Left Side)
   
   - Close: Space Orange (#E57C3A) - 12px diameter
   - Minimize: Mars Gold (#D4A93E) - 12px diameter  
   - Maximize/Restore: Star White border - 12px diameter
   - Hover: subtle brightness increase
   - Click: smooth animation

3. **Window Border**
   
   - 1px subtle border
   - Corner radius: 8px
   - Consistent across all windows

**Technical:**

- Server-side decorations (compositor draws them)
- Integrated with TilingManager
- EventBus for button interactions
- Animation engine for hover/click effects

### Tiling Manager

**Built Into openSEF Framework** (Not a plugin or extension)

**Features:**

1. **Automatic Tiling**
   
   - Drag window to edge: suggests tile position
   - Keyboard shortcuts for quick tiling
   - Multiple layout modes:
     - Half-screen (left/right)
     - Quarter-screen (corners)
     - Thirds (side + center)
     - Custom layouts

2. **Smart Behavior**
   
   - Remembers per-app tiling preferences
   - Smooth animations during tile transitions
   - Respects minimum window sizes
   - Works seamlessly with custom decorations

3. **Visual Feedback**
   
   - Highlight overlay shows tile preview
   - Smooth snap animation
   - Elastic resize animations

**Technical:**

- Part of openSEF WindowManager
- Uses animation engine for transitions
- Stores state in StateManager
- No external protocols needed

---

## Cockpit View

**Official Name: Cockpit View**
*(NOT "Multitask View" or "Activities" - those are old Linux DE paradigms)*

### Purpose

Complete overview and control center for all virtual desktops and windows. Named "Cockpit" because it gives you total control like an aircraft cockpit - see everything, control everything.

### Activation

**Methods:**

1. Click the **Orange Box** button (top-left corner of panel)
2. Keyboard shortcut (e.g., Super key or F3)

**Exit:**

- Press `Esc` to return to active desktop
- Click on any window card
- Click on any desktop button in sidebar

### Exact Layout (Based on Screenshot)

```
┌─────────────────────────────────────────────────────────────────┐
│  orange|                                                        |
|    box |  Filer  Edit  View  Help                      [Clock]  │ ← Panel (stays visible)
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ┌────┐    Active Window                                         │
│  │ ⚫ │                                                          |
│  ├────┤  ┌───────────────────┐  ┌───────────────────┐            │
│  │ ⚪ │  │                   │  │                   │           │
│  ├────┤  │    ┌───────┐     │  │    ┌───────┐       │           │
│  │ ⚪ │  │    │   C   │     │  │    │   C   │       │  ...      │
│  └────┘  │    └───────┘     │  │    └───────┘       │           │
│          │                   │  │                   │           │
│    ↑     │      Filer        │  │      Safari       │           │
│ Desktop  │  com.vitusos.filer│  │  com.vitusos.zen- |           |
|          |                   |  |      browser      │           │
│ Sidebar  │                   │  │                   │           │
|  (pill   |                   |  |                   |           |      
|   like   |                   |  |                   |           |     
|   shape, |                   |  |                   |           |
|   tall)  └───────────────────┘  └───────────────────┘           │
│                                                                 │
│                    [Mars Wallpaper - Slightly dimmed]           │
└─────────────────────────────────────────────────────────────────┘
```

### Component Breakdown

#### 1. Left Sidebar - Desktop Switcher

**Design:**

- Vertical column of circular buttons
- Each button represents one virtual desktop
- Fixed width: ~64px
- Buttons: ~48px diameter circles
- Spacing: 8px between buttons

**Visual States:**

- **Active Desktop**: Filled circle (⚫) - solid color
- **Inactive Desktop**: Empty circle (⚪) - outline only
- **Hover**: Subtle scale (1.0 → 1.1) + fill animation

**Behavior:**

- Click any desktop button to switch to that desktop
- Active desktop highlighted
- Smooth transition animation when switching
- Scrollable if too many desktops to fit vertically

**Position:**

- Left edge of screen
- Starts below panel
- Extends down toward dock
- Always visible in Cockpit View

#### 2. Window Cards - Main Area

**Card Design:**

```
┌───────────────────────┐
│                       │
│   ┌─────────────┐     │
│   │             │     │  ← Large circular app icon
│   │      C      │     │     (64-80px diameter)
│   │             │     │
│   └─────────────┘     │
│                       │
│    Application Name   │  ← Inter Medium, 16px
│   com.bundle.id       │  ← Inter Regular, 12px, muted
│                       │
└───────────────────────┘
```

**Card Specifications:**

- Width: ~280-320px (generous sizing)
- Height: ~200-240px
- Corner radius: 8px
- Background: Star White (#FFFFFF) or Lunar Gray (#F5F5F5)
- Subtle shadow: 0 4px 12px rgba(0,0,0,0.08)
- Padding: 24px

**Card Content:**

1. **Large App Icon** (top, centered)
   
   - Circular icon: 64-80px diameter
   - In screenshot: placeholder "C" circle
   - Production: actual app icon

2. **App Name** (below icon, centered)
   
   - Font: Inter Medium
   - Size: 16px
   - Color: Deep Space (#1A1A1A)
   - Example: "Filer", "Safari", "Terminal"

3. **Bundle Identifier** (below name, centered)
   
   - Font: Inter Regular
   - Size: 12px
   - Color: Cosmic Gray (#808080) - muted
   - Example: "com.vitusos.filer"

**Card Layout:**

- Arranged horizontally in rows
- Spacing between cards: 24-32px
- Multiple rows if many windows
- Centered in available space (right of sidebar)

**Card Animations:**

- **Entrance**: Fade in (0 → 1 opacity) + slight scale (0.95 → 1.0)
- **Hover**: Scale 1.0 → 1.05 + shadow increase
- **Click**: Quick scale 1.05 → 0.98 → exit Cockpit View
- **Exit**: Fade out + scale down
- Timing: 250ms with ease-out
- **Windows 8 tile inspiration**: Cards feel alive, responsive

#### 3. Background

**Appearance:**

- Mars wallpaper visible
- Dimmed: 60-70% opacity or darkened overlay
- Provides context without distraction
- Cards clearly visible against background

#### 4. Panel Behavior

**During Cockpit View:**

- Panel stays visible at top (NOT hidden)
- Shows current app menu (from focused app before entering)
- Orange Box button accessible (can re-click to exit)
- Clock and system tray remain visible
- Slightly dimmed or blurred to emphasize cards

### Interaction Flow

1. **Entering Cockpit View:**
   
   ```
   User clicks Orange Box (or presses shortcut)
           ↓
   Screen zooms out slightly
           ↓
   Window cards animate in (fade + scale)
           ↓
   Desktop sidebar appears on left
           ↓
   Background dims
           ↓
   User has full overview
   ```

2. **Switching Desktops:**
   
   ```
   User clicks different desktop button in sidebar
           ↓
   Current cards fade out
           ↓
   New desktop's cards fade in
           ↓
   Active desktop indicator updates
   ```

3. **Selecting Window:**
   
   ```
   User clicks window card
           ↓
   Card scales down briefly
           ↓
   All cards fade out
           ↓
   Screen zooms back to normal
           ↓
   Selected window focused and active
   ```

4. **Exiting Without Selection:**
   
   ```
   User presses Esc (or clicks Orange Box again)
           ↓
   Cards fade out
           ↓
   Screen returns to previous state
           ↓
   Last active window remains focused
   ```

### Performance Requirements

**CRITICAL: Must Be Lightweight**

❌ **Don't:**

- Re-render entire screen every frame
- Use heavy compositing effects
- Generate high-res screenshots constantly
- Block compositor during view transitions

✅ **Do:**

- Use efficient card rendering
- Render at appropriate resolution
- Update only when windows change
- Smooth 60 FPS animations
- Instant response to user input

**Technical Approach:**

- Cache window information (update on changes)
- Vulkan-accelerated rendering via QRhi
- Minimize overdraw
- Efficient scene graph
- No heavy lifting - should feel instant
- Focus on icon + metadata display, not full window thumbnails

---

## Application Framework

### Native App Development (GNUstep AppKit C++ Fork)

**The Goal:**
Create a native widget framework for openSEF apps that feels like developing for macOS with AppKit.

**Current Status (uptc):**

- GNUstep AppKit being forked
- Converting from Objective-C to C++
- Integrating with openSEF APIs
- Building widget library

**Future (up1):**

- Complete C++ AppKit fork
- All native apps use this framework
- No Qt/GTK dependencies for native apps

### Widget Library

**Core Widgets:**

- OSFWindow (application windows)
- OSFButton (standard buttons)
- OSFTextField (text input)
- OSFLabel (text display)
- OSFPanel (container with glass effect)
- OSFMenu (menus and menu items)
- OSFToolbar (app toolbars)
- OSFTableView (list/table data)
- OSFImageView (image display)
- OSFProgressBar (progress indicators)

**Design Principles:**

- Consistent Ares visual design
- Smooth animations built-in
- Auto-integrates with global menu
- Respects system theme
- Accessibility support

### Native Applications

#### 1. Filer (File Manager)

**Special Status:**

- **Always running** (like macOS Finder)
- Always shows dot in dock
- Cannot be quit from GUI (can relaunch if crashed)
- Provides file browsing for all apps

**Features:**

- Column view (Miller columns, like Finder)
- Icon view with customizable sizes
- List view with sortable columns
- Quick Look preview (spacebar)
- Sidebar with favorites and devices
- Tabbed browsing
- Advanced search
- File tagging and metadata
- Network browsing (SMB, NFS)
- Integration with Universal Clipboard
- Seamless SeaDrop integration

**Better Than Finder:**

- Faster search
- More keyboard shortcuts
- Better network performance
- Customizable views
- Advanced batch operations

#### 2. SeaDrop (AirDrop-like File Sharing)

**Purpose:**
Seamless file sharing between VitusOS Ares machines (and potentially other platforms)

**Features:**

- Zero-configuration networking
- Encrypted transfers
- Universal Clipboard integration
- Drag-and-drop sharing
- Contact-based sharing
- Works across local network
- Fast and secure

**Integration:**

- Built into Filer (share button)
- System-wide share menu
- Clipboard interception
- Background service

#### 3. Terminow (Terminal Emulator)

**Features:**

- GPU-accelerated rendering
- Tabs and split panes
- Configurable key bindings
- True color support
- Smooth scrolling
- Font ligature support
- Shell integration

#### 4. Settings (System Preferences)

**Organization:**

- Clean, searchable interface
- Organized by category
- Per-app preferences
- System-wide settings
- User account management
- Network configuration
- Display settings
- Accessibility options

---

## System Integration

### Universal Clipboard

**What It Is:**
System-wide clipboard that seamlessly integrates with all apps and services.

**Features:**

1. **Shared State**
   
   - All apps access same clipboard via OSFDesktop
   - Copy in one app, paste in any other
   - Clipboard history (recent items)
   - Multiple data formats (text, images, files)

2. **SeaDrop Integration**
   
   - Copy on one machine, paste on another
   - Seamless cross-device clipboard
   - Encrypted synchronization
   - Works automatically on local network

3. **Smart Behavior**
   
   - Understands data types
   - Preserves formatting
   - Handles large files efficiently
   - Security: apps request clipboard access

**Technical:**

- Managed by ClipboardManager in openSEF
- EventBus notifications for clipboard changes
- Support for standard MIME types
- Integration with Wayland clipboard protocols

### Global Menu System

**Purpose:**
Extract menus from all applications and display in panel's global menu bar.

**Support Matrix:**

| App Type                | Integration Method   | Status                |
| ----------------------- | -------------------- | --------------------- |
| **Native openSEF Apps** | Direct API (OSFMenu) | ✅ Full support        |
| **GTK Applications**    | DBusMenu protocol    | ✅ Full support (uptc) |
| **Qt Applications**     | DBusMenu protocol    | ✅ Full support (uptc) |
| **Electron Apps**       | DBusMenu protocol    | ✅ Full support (uptc) |
| **Legacy X11 Apps**     | Best effort          | ⚠️ Limited            |

**How It Works (uptc - Current):**

1. Application registers menu via DBusMenu
2. Panel's menu component subscribes to changes
3. Menu items displayed in global menu bar
4. Click events sent back to application
5. Application updates menu, panel reflects changes

**How It Will Work (up1 - Future):**

1. Native apps use OSFMenu API directly
2. No protocols - direct C++ API calls
3. Even faster, more integrated
4. Legacy apps still use DBusMenu for compatibility

### System Tray (Background Services)

**Purpose:**
Display and control background apps and system services.

**Contents:**

1. **System Services:**
   
   - Network (WiFi, Ethernet status)
   - Battery (level, charging status)
   - Volume (audio level, mute)
   - Bluetooth (connection status)
   - Display (brightness control)

2. **Background Apps:**
   
   - Cloud sync services
   - Chat applications (minimized)
   - Media players (hidden)
   - System monitors
   - Update services

**Interaction:**

- Click tray icon to reveal dropdown
- Dropdown shows all services/apps
- Click item to control/show
- Quick actions (connect WiFi, adjust volume)
- Smooth animations

**Technical:**

- StatusNotifier protocol (uptc)
- Native openSEF API (up1)
- Managed by Panel component
- EventBus for status updates

---

## Design System

### Ares Color Palette

The Mars-inspired color scheme that defines VitusOS Ares visual identity.

| Color Name       | Hex Code  | RGB                | Usage                                         |
| ---------------- | --------- | ------------------ | --------------------------------------------- |
| **Space Orange** | `#E57C3A` | rgb(229, 124, 58)  | Primary accent, close button, active elements |
| **Mars Gold**    | `#D4A93E` | rgb(212, 169, 62)  | Secondary accent, minimize button, highlights |
| **Star White**   | `#FFFFFF` | rgb(255, 255, 255) | Window backgrounds, primary surfaces          |
| **Deep Space**   | `#1A1A1A` | rgb(26, 26, 26)    | Primary text, icons                           |
| **Lunar Gray**   | `#F5F5F5` | rgb(245, 245, 245) | Panels, title bars, subtle backgrounds        |
| **Cosmic Gray**  | `#E0E0E0` | rgb(224, 224, 224) | Borders, dividers                             |
| **Mars Dust**    | `#F4E7D7` | rgb(244, 231, 215) | Subtle highlights, hover states               |

### UI Dimensions

| Element             | Size          | Corner Radius | Notes                         |
| ------------------- | ------------- | ------------- | ----------------------------- |
| **Panel**           | Height: 28px  | 0px           | Top of screen, no rounding    |
| **Dock**            | Height: 64px  | 16px          | Bottom, rounded corners       |
| **Window Border**   | 1px           | 8px           | Consistent across all windows |
| **Traffic Lights**  | 12px diameter | 6px (circle)  | Window control buttons        |
| **Button (Small)**  | Height: 24px  | 4px           | Toolbar buttons               |
| **Button (Medium)** | Height: 32px  | 6px           | Standard buttons              |
| **Button (Large)**  | Height: 40px  | 8px           | Primary actions               |
| **Text Field**      | Height: 28px  | 4px           | Standard input                |
| **Panel Items**     | Padding: 8px  | N/A           | Spacing between elements      |
| **Window Padding**  | 16px          | N/A           | Content margin                |

### Typography

**System Font:**

- **All UI Elements**: Inter font family
- **Terminal Only**: Monospace font (NOT Inter)

**Inter Font Weights:**

- Regular (400): Body text, labels, general UI
- Medium (500): Buttons, emphasized UI elements
- Semibold (600): Headings, section titles
- Bold (700): Strong emphasis, important text

**Font Sizes:**

- Heading 1: 24px, Bold
- Heading 2: 18px, Semibold
- Body: 14px, Regular
- Caption: 12px, Regular
- Small: 10px, Regular

**Why Inter:**

- Modern, clean sans-serif designed for UI
- Excellent screen readability at all sizes
- Professional, contemporary appearance
- Consistent across entire openSEF interface
- Exception: Terminal uses monospace for code/output

### Animation Principles

**Timing:**

- Quick interactions: 150ms
- Standard transitions: 250ms
- Page transitions: 350ms
- Emphasis: 500ms

**Easing:**

```
Standard: cubic-bezier(0.4, 0.0, 0.2, 1)
Decelerate: cubic-bezier(0.0, 0.0, 0.2, 1)
Accelerate: cubic-bezier(0.4, 0.0, 1, 1)
```

**Key Principles:**

1. **Purposeful** - Every animation serves a function
2. **Smooth** - 60 FPS minimum, prefer 120 FPS
3. **Consistent** - Same elements animate the same way
4. **Fast** - Don't slow users down
5. **Natural** - Physics-based when appropriate

### Consistent UI/UX Across All Elements

**CRITICAL REQUIREMENT:**
Every UI element in VitusOS Ares MUST follow the design system.

**This Includes:**

1. **System Dialogs**
   
   - File pickers
   - Save/Open dialogs
   - Alert messages
   - Confirmation prompts
   - Error messages

2. **Pop-up Windows**
   
   - Context menus
   - Dropdown menus
   - Tooltips
   - Notifications
   - Quick look panels

3. **Application Windows**
   
   - Title bars
   - Toolbars
   - Status bars
   - Sidebars
   - Content areas

4. **Interactive Elements**
   
   - Buttons (all states: normal, hover, active, disabled)
   - Text fields
   - Checkboxes
   - Radio buttons
   - Sliders
   - Progress bars

**No Exceptions:**

- No default Qt dialogs
- No GTK theme leaking through
- No inconsistent button styles
- No mixed animation timings
- Everything follows Ares design system

**Visual Consistency Examples:**

✅ **Correct:**

```
All buttons have 6-8px corner radius
All hover states use 150ms transition
All shadows use same blur/offset
All text uses system font stack
All colors from Ares palette
```

❌ **Wrong:**

```
Some buttons square, others rounded
Random transition timings
Inconsistent shadow styles
Mixed fonts (Arial vs Helvetica vs ...)
Random colors not in palette
```

---

## Release Channels

### uptc (Upstream Color) - Current Development Channel

**Status**: Active Development / Early Access

**Philosophy:**
Build a functional desktop using existing Linux protocols for compatibility while developing the unified openSEF framework.

**What's In uptc:**

1. **Complete Desktop Environment**
   
   - ✅ Compositor (wlroots + Vulkan) - Working
   - ✅ Shell (Qt Quick + Vulkan) - Working
   - ✅ Panel with global menu - Working
   - ✅ Dock with intelligence - Working
   - 🚧 Multitask View - In Progress
   - 🚧 Window tiling - In Progress
   - 🚧 Custom decorations - In Progress

2. **Compatibility Layer**
   
   - ✅ DBusMenu for global menus
   - ✅ StatusNotifier for system tray
   - ✅ Standard Wayland protocols
   - ✅ GTK/Qt app support

3. **GNUstep AppKit Work**
   
   - 🚧 Forking GNUstep AppKit
   - 🚧 Converting Objective-C to C++
   - 🚧 Integration with openSEF APIs
   - 📋 Widget library development

4. **Native Apps**
   
   - 🚧 Filer (basic functionality)
   - 📋 SeaDrop (thesis project)
   - 📋 Terminow
   - 📋 Settings

**Timeline:**

- Active development through 2026
- Goal: Functional, daily-usable desktop
- Focus: Getting things working with good UX

**Target Users:**

- Early adopters
- Developers
- Testers and contributors
- Anyone willing to deal with rough edges

### up1 (Upstream One) - Future Stable Release

**Status**: Planned

**Philosophy:**
Pure openSEF unified framework with no dependency on Linux protocols. Everything native, everything integrated.

**What up1 Will Be:**

1. **Complete Native Framework**
   
   - ✅ GNUstep AppKit fully converted to C++
   - ✅ All widgets native to openSEF
   - ✅ No DBusMenu - native menu API
   - ✅ No StatusNotifier - native tray API
   - ✅ Everything goes through OSFDesktop::shared()

2. **Zero Protocol Fragmentation**
   
   - No D-Bus for internal communication
   - No freedesktop.org protocols for native apps
   - Pure C++ APIs throughout
   - Event-driven via EventBus only

3. **Complete Boot-to-Shutdown Control**
   
   - ✅ Boot animation (systemd completely hidden)
   - ✅ Session management
   - ✅ Service management
   - ✅ Smooth shutdown animation

4. **Mature Native Apps**
   
   - ✅ Filer (feature-complete)
   - ✅ SeaDrop (stable, cross-platform)
   - ✅ Terminow (GPU-accelerated)
   - ✅ Settings (all system config)
   - ✅ Additional native apps

**Backwards Compatibility:**

- GTK/Qt/Electron apps still supported
- They use compatibility protocols (DBusMenu, etc.)
- Native apps use pure openSEF APIs
- Best experience with native apps

**Timeline:**

- After uptc is stable and mature
- Depends on GNUstep AppKit conversion completion
- Focus: Polish, stability, performance
- Goal: Production-ready desktop for everyone

**Target Users:**

- Everyone
- Daily drivers
- Professional use
- General public

---

## Development Phases

### Phase 1: Framework Foundation ✅ COMPLETE

**Goal:** Build the core openSEF framework architecture

**Completed:**

- [x] OSFDesktop central API
- [x] EventBus (publish/subscribe system)
- [x] StateManager (centralized state)
- [x] Basic project structure
- [x] Build system (CMake + Nix)

**Timeline:** December 2025

### Phase 2: Windowing Model ✅ COMPLETE

**Goal:** Get windows displaying and managed

**Completed:**

- [x] wlroots compositor integration
- [x] Wayland protocol server
- [x] XDG shell for client windows
- [x] Layer shell for panels/dock
- [x] Basic window management
- [x] Input handling (keyboard/mouse)
- [x] Scene graph rendering

**Timeline:** December 2025

### Phase 3: Shell & Layout ✅ COMPLETE

**Goal:** Display the desktop shell

**Completed:**

- [x] Qt Quick shell integration
- [x] Panel rendering (global menu bar)
- [x] Dock rendering (app launcher)
- [x] Wallpaper system
- [x] Basic layout management
- [x] Vulkan rendering via QRhi

**Timeline:** December 2025 - January 2026

### Phase 4: Controls & Integration 🚧 CURRENT (uptc)

**Goal:** Make everything interactive and functional

**In Progress:**

- [x] Window focus and switching
- [ ] Global menu (DBusMenu integration)
- [ ] System tray (StatusNotifier functional)
- [ ] Dock intelligence (auto-hide, app tracking)
- [ ] Cockpit View implementation (virtual desktop overview)
- [ ] Window tiling system
- [ ] Custom window decorations
- [ ] Animation polish

**Timeline:** January 2026 - Q2 2026

### Phase 5: System Services 📋 NEXT

**Goal:** Full system integration

**Planned:**

- [ ] Boot manager (hide systemd)
- [ ] Session management
- [ ] Login screen
- [ ] Power management
- [ ] Network management
- [ ] Audio management
- [ ] Display configuration
- [ ] User management

**Timeline:** Q2 2026 - Q3 2026

### Phase 6: GNUstep AppKit Fork 📋 PLANNED

**Goal:** Native widget framework for apps

**Planned:**

- [ ] Fork GNUstep AppKit repository
- [ ] Convert Objective-C to C++
- [ ] Integrate with openSEF APIs
- [ ] Build widget library
- [ ] Create app templates
- [ ] Documentation and examples

**Timeline:** Q3 2026 - Q4 2026

### Phase 7: Native Applications 📋 PLANNED

**Goal:** Complete native app ecosystem

**Planned Apps:**

1. **Filer (Priority 1)**
   
   - File manager
   - Always running (like Finder)
   - Column/icon/list views
   - Search and tagging
   - Network browsing

2. **SeaDrop (Priority 1 - Thesis Project)**
   
   - AirDrop-like sharing
   - Zero-config networking
   - Universal Clipboard integration
   - Cross-platform support

3. **Terminow (Priority 2)**
   
   - Terminal emulator
   - GPU acceleration
   - Tabs and splits
   - Shell integration

4. **Settings (Priority 2)**
   
   - System preferences
   - All configuration
   - User management
   - Network/display/audio

5. **Additional Apps (Priority 3)**
   
   - Text editor
   - Image viewer
   - PDF reader
   - Calculator
   - Notes app

**Timeline:** Q4 2026 - Q1 2027

### Phase 8: Final Polish 📋 PLANNED

**Goal:** Production-ready release (up1)

**Planned:**

- [ ] Performance optimization
- [ ] Accessibility features
- [ ] Internationalization (i18n)
- [ ] Documentation complete
- [ ] User guides and tutorials
- [ ] Bug fixes and stability
- [ ] Security audit
- [ ] Release preparation

**Timeline:** Q1 2027 - Q2 2027

---

## Critical Success Factors

### What Makes VitusOS Ares Different (and Better)

1. **True Unity**
   
   - Not a collection of apps, ONE system
   - Everything speaks same language
   - Shared resources and state
   - Consistent behavior everywhere

2. **User Experience First**
   
   - Smooth animations (60+ FPS)
   - Intelligent behavior (auto-hide dock)
   - Beautiful design (Ares aesthetic)
   - No configuration needed

3. **Developer Experience**
   
   - Clean APIs (like Cocoa)
   - Good documentation
   - Modern C++ (not C with GTK)
   - Easy to build apps

4. **Performance**
   
   - Vulkan GPU acceleration
   - Efficient rendering (QRhi)
   - Lightweight (no bloat)
   - Fast boot and shutdown

5. **Compatibility**
   
   - Run existing Linux apps
   - Global menu for all apps
   - System tray support
   - Seamless integration

### Non-Negotiable Principles

**MUST HAVES:**

1. **Unified Framework**
   
   - Everything uses OSFDesktop::shared()
   - No fragmentation
   - One source of truth

2. **Visual Consistency**
   
   - Every UI element follows design system
   - No exceptions
   - Ares palette everywhere

3. **Smooth Performance**
   
   - 60 FPS minimum
   - Instant response
   - No stuttering or lag

4. **Hidden Complexity**
   
   - Users never see systemd
   - No technical jargon in UI
   - It just works

5. **Beautiful Design**
   
   - Mars-inspired aesthetic
   - Warm and inviting
   - Polished and professional

**MUST NOT HAVES:**

1. **Fragmentation**
   
   - No D-Bus spaghetti for internal comms
   - No protocol soup
   - No component independence

2. **Ugly Technical Details**
   
   - No systemd logs visible
   - No config file editing required
   - No command line for basic tasks

3. **Inconsistency**
   
   - No random button styles
   - No mixed fonts
   - No varied animation timings

4. **Complexity**
   
   - No overwhelming users
   - No 50 configuration options
   - No manual setup

---

## Technical Deep Dives

### Compositor Architecture (opensef-compositor)

**Files (Pure C):**

```
opensef/opensef-compositor/src/
├── main.c              # Entry point, argument parsing
├── server.c            # wlroots initialization, main loop
├── output.c            # Monitor/display handling
├── view.c              # Window (XDG surface) management
├── input.c             # Keyboard and mouse handling
├── layer_shell.c       # wlr-layer-shell for panels/dock
├── decorations.c       # Server-side window decorations
└── cursor.c            # Cursor rendering and management
```

**Core Data Structures:**

```c
struct osf_server {
    struct wl_display *wl_display;
    struct wlr_backend *backend;
    struct wlr_renderer *renderer;
    struct wlr_scene *scene;

    struct wlr_xdg_shell *xdg_shell;
    struct wlr_layer_shell_v1 *layer_shell;

    struct wl_list outputs;  // linked list of osf_output
    struct wl_list views;    // linked list of osf_view

    struct wlr_cursor *cursor;
    struct wlr_seat *seat;
};

struct osf_output {
    struct wl_list link;
    struct osf_server *server;
    struct wlr_output *wlr_output;
    struct wlr_scene_output *scene_output;
};

struct osf_view {
    struct wl_list link;
    struct osf_server *server;
    struct wlr_xdg_toplevel *xdg_toplevel;
    struct wlr_scene_tree *scene_tree;

    int x, y;
    uint32_t width, height;
};
```

**Why Pure C:**

- wlroots uses C99-only features (`[static 4]`)
- C++ compilers reject this syntax
- Must be pure C for compatibility
- Same approach as labwc, sway, wayfire

**Rendering Pipeline:**

```
Application draws to Wayland surface
         ↓
wlroots scene graph manages surfaces
         ↓
Vulkan renderer composites scene
         ↓
Output to display(s)
```

### Shell Architecture (opensef-shell)

**Files (C++ + Qt Quick):**

```
opensef/opensef-shell/src/
├── main.cpp                    # Entry point, QGuiApplication
├── shell/
│   ├── ShellManager.cpp       # Coordinates all shell components
│   ├── Panel.cpp              # Top panel/menu bar
│   ├── Dock.cpp               # Bottom application dock
│   └── Wallpaper.cpp          # Background rendering
├── widgets/
│   ├── OSFButton.cpp          # Button widget
│   ├── OSFLabel.cpp           # Label widget
│   └── OSFTextField.cpp       # Text input widget
├── render/
│   ├── VulkanRenderer.cpp     # Vulkan via QRhi
│   └── SceneGraph.cpp         # Qt Quick scene graph
└── wayland/
    └── LayerShellIntegration.cpp  # wlr-layer-shell client
```

**QML Files:**

```
opensef/opensef-shell/qml/
├── Panel.qml                  # Panel layout and menu
├── Dock.qml                   # Dock layout and icons
├── MultitaskView.qml          # Virtual desktop overview
└── components/
    ├── GlobalMenu.qml         # Menu bar component
    ├── SystemTray.qml         # System tray dropdown
    └── Clock.qml              # Clock widget
```

**Qt Quick + Vulkan Pipeline:**

```
QML describes UI layout
         ↓
Qt Quick compiles to scene graph
         ↓
QRhi abstracts GPU rendering
         ↓
Vulkan backend renders at 60+ FPS
         ↓
wlr-layer-shell presents to compositor
```

**Why Qt Quick:**

- Declarative UI (rapid iteration)
- GPU-accelerated (600+ FPS capable)
- QRhi abstraction (Vulkan/Metal/D3D)
- Modern rendering pipeline
- Great for shell UI

### Framework Architecture (opensef-framework)

**Core Classes:**

```cpp
// Central API - Singleton pattern
class OSFDesktop {
public:
    static OSFDesktop& shared();

    EventBus& eventBus();
    StateManager& stateManager();
    WindowManager& windowManager();
    ClipboardManager& clipboardManager();
    AnimationEngine& animationEngine();
    ThemeEngine& themeEngine();
    TilingManager& tilingManager();

private:
    OSFDesktop();
    // ... managers
};

// Publish/subscribe event system
class EventBus {
public:
    void publish(const Event& event);
    void subscribe(const std::string& eventType, 
                   EventHandler handler);
    void unsubscribe(const std::string& eventType, 
                     EventHandler handler);
};

// Centralized state management
class StateManager {
public:
    void setState(const std::string& key, 
                  const std::any& value);
    std::any getState(const std::string& key);
    void observeState(const std::string& key, 
                      StateObserver observer);
};

// Window lifecycle and positioning
class WindowManager {
public:
    void addWindow(Window* window);
    void removeWindow(Window* window);
    void focusWindow(Window* window);
    void tileWindow(Window* window, TileMode mode);
    std::vector<Window*> getWindows();
};
```

**Event Flow Example:**

```cpp
// App launches
app->launch();
    ↓
// Publishes event
OSFDesktop::shared().eventBus()
    .publish(AppLaunchEvent{"com.vitusos.filer"});
    ↓
// Dock subscribes to these events
Dock subscribes: AppLaunchEvent
    ↓
// Dock receives notification
dock->onAppLaunched(event);
    ↓
// Dock updates UI (shows dot indicator)
dock->updateIcon("com.vitusos.filer", /*active=*/true);
```

**State Management Example:**

```cpp
// Store window positions
OSFDesktop::shared().stateManager()
    .setState("window.positions", windowPositions);

// Later, retrieve them
auto positions = OSFDesktop::shared().stateManager()
    .getState("window.positions");

// Observe changes
OSFDesktop::shared().stateManager()
    .observeState("theme.mode", [](const std::any& value) {
        bool darkMode = std::any_cast<bool>(value);
        updateTheme(darkMode);
    });
```

### GNUstep AppKit Integration (Future - up1)

**Current GNUstep (Objective-C):**

```objc
@interface MyWindow : NSWindow
@end

@implementation MyWindow
- (void)someMethod {
    [self setTitle:@"Hello"];
}
@end
```

**After C++ Conversion:**

```cpp
class OSFWindow : public OSFResponder {
public:
    void setTitle(const std::string& title);
    void setContentView(OSFView* view);
    void makeKeyAndOrderFront();

private:
    std::string m_title;
    OSFView* m_contentView;
};
```

**Integration with openSEF:**

```cpp
// GNUstep widget automatically integrates
OSFWindow* window = new OSFWindow();

// Menus automatically go to global menu bar
OSFMenu* menu = new OSFMenu("File");
menu->addItem("New", []() { /* ... */ });

// Window decorations handled by compositor
window->setHasCustomDecorations(false);

// Everything goes through unified framework
OSFDesktop::shared().windowManager()
    .addWindow(window);
```

---

## Debugging and Development

### Building from Source

**Prerequisites:**

```bash
# NixOS (recommended)
nix develop

# Or manually install:
- wlroots 0.19+
- Wayland
- Qt 6.x with Quick and Vulkan support
- CMake 3.20+
- Ninja
- GCC/Clang with C++20 support
```

**Build Steps:**

```bash
# 1. Clone repository
git clone https://github.com/raven1zed/VitusOS-Ares.git
cd VitusOS-Ares

# 2. Enter development environment (NixOS)
nix develop

# 3. Build compositor
cd opensef/opensef-compositor
mkdir -p build && cd build
cmake .. -G Ninja
ninja

# 4. Build shell (from project root)
cd ../../opensef-shell
mkdir -p build && cd build
cmake .. -G Ninja
ninja

# 5. Build framework
cd ../../opensef-framework
mkdir -p build && cd build
cmake .. -G Ninja
ninja
```

**Running (Nested in Existing Session):**

```bash
# Terminal 1: Run compositor
cd opensef/opensef-compositor/build
WLR_BACKENDS=wayland ./opensef-compositor

# Terminal 2: Run shell
cd opensef/opensef-shell/build
./opensef-shell

# Terminal 3: Test apps
weston-terminal  # or any Wayland app
```

### Common Issues and Solutions

**Issue: Compositor won't start**

```
Error: Failed to create backend
Solution: Make sure you're running in a Wayland session
         Use WLR_BACKENDS=wayland for nested mode
```

**Issue: Shell doesn't appear**

```
Error: Failed to connect to compositor
Solution: Ensure compositor is running first
         Check WAYLAND_DISPLAY environment variable
```

**Issue: Black screen**

```
Error: Vulkan initialization failed
Solution: Install proper Vulkan drivers
         Check vulkaninfo output
         Try WLR_RENDERER=pixman for software rendering
```

**Issue: Segmentation fault in compositor**

```
Error: Crash in wlroots code
Solution: Verify wlroots version (need 0.19+)
         Check for NULL pointer dereferences
         Enable debug symbols: cmake -DCMAKE_BUILD_TYPE=Debug
```

### Logging and Debugging

**Enable Verbose Logging:**

```bash
# Compositor
WLR_DEBUG=1 ./opensef-compositor

# Shell
QT_LOGGING_RULES="*.debug=true" ./opensef-shell

# Combined
WLR_DEBUG=1 WLR_BACKENDS=wayland \
QT_LOGGING_RULES="*.debug=true" \
./run_complete_system.sh
```

**Debugging with GDB:**

```bash
# Compositor
gdb --args ./opensef-compositor
(gdb) run
(gdb) bt  # backtrace on crash

# Shell
gdb --args ./opensef-shell
(gdb) run
```

**Valgrind Memory Check:**

```bash
valgrind --leak-check=full \
         --track-origins=yes \
         ./opensef-compositor
```

---

## FAQ (Frequently Asked Questions)

### General Questions

**Q: Is VitusOS Ares a Linux distribution?**
A: Yes, it's a complete Linux distribution based on NixOS, but with a unified desktop experience like macOS rather than traditional Linux DEs.

**Q: Can I run existing Linux applications?**
A: Yes! GTK, Qt, and Electron apps are fully supported with global menu integration.

**Q: Why not just use GNOME/KDE?**
A: Traditional DEs are fragmented collections of components. VitusOS Ares is ONE unified system where everything speaks the same language through openSEF.

**Q: Is this just another Wayland compositor?**
A: No. openSEF includes a compositor, but it's much more - it's a complete framework controlling everything from boot to shutdown.

### Technical Questions

**Q: Why hybrid C/C++?**
A: wlroots (compositor) must be pure C due to C99-only syntax. Shell and framework are C++ for better architecture. This is standard practice (labwc, sway do this too).

**Q: Why Qt Quick instead of GTK?**
A: Qt Quick provides 600+ FPS GPU-accelerated rendering via QRhi (Vulkan/Metal/D3D), perfect for smooth animations and modern UI.

**Q: Why fork GNUstep instead of using Qt everywhere?**
A: GNUstep provides macOS-like widget behavior and APIs. We're converting it to C++ to integrate perfectly with openSEF while keeping the excellent AppKit design.

**Q: What about X11 applications?**
A: XWayland support for legacy X11 apps, but they won't have global menus or full integration.

### Development Questions

**Q: How can I contribute?**
A: Check the GitHub repository for open issues, read the documentation, and submit PRs. We welcome contributions!

**Q: What's the development timeline?**
A: uptc (current) is active through 2026. up1 (stable) planned for 2027.

**Q: Can I use VitusOS Ares as my daily driver?**
A: Not yet for uptc (early development). Wait for up1 for production use.

**Q: Is there a live ISO I can try?**
A: Not yet. Currently requires building from source. ISO releases planned for up1.

### Design Questions

**Q: Can I customize the colors?**
A: Eventually yes, but the Ares palette is core to the identity. Customization will maintain design consistency.

**Q: Will there be a dark mode?**
A: Yes, planned for up1 with proper dark variants of all colors.

**Q: Can I resize the panel/dock?**
A: No. Fixed dimensions ensure consistent design. This is intentional (like macOS).

---

## Conclusion

### The Vision Realized

VitusOS Ares with openSEF represents a fundamental rethinking of what a Linux desktop can be.

**Not fragmentation, but unity.**
**Not protocols, but APIs.**
**Not components, but a system.**

### What Success Looks Like

When VitusOS Ares is complete (up1):

- User powers on their machine
- Sees beautiful Ares boot animation (no systemd logs)
- Logs in smoothly
- Desktop appears instantly with warm Mars aesthetic
- All apps show menus in global menu bar
- Dock intelligently manages running applications
- Multitask View provides perfect overview of workspaces
- Windows tile smoothly with animations
- Universal Clipboard works seamlessly
- Everything feels like ONE cohesive system
- User powers off with smooth shutdown animation

**It just works. It's beautiful. It's unified.**

### For Developers

Building an app for openSEF (in up1):

```cpp
#include <opensef/OSFDesktop.h>
#include <opensef/OSFWindow.h>
#include <opensef/OSFButton.h>

int main(int argc, char** argv) {
    OSFApplication app(argc, argv);

    OSFWindow* window = new OSFWindow();
    window->setTitle("My App");

    OSFButton* button = new OSFButton("Click Me");
    button->onClick([]() {
        OSFDesktop::shared().eventBus()
            .publish(Event{"my.app.button.clicked"});
    });

    window->setContentView(button);
    window->show();

    return app.exec();
}
```

**That's it. No boilerplate. No configuration. Just code.**

### For Users

Using VitusOS Ares:

- Install (eventually: just flash ISO and boot)
- Use (click, type, enjoy)
- Customize (if desired, through Settings app)
- Update (seamless, no reinstall needed)

**No terminal required. No config files to edit. It just works.**

---

## Final Words

This is the complete vision for VitusOS Ares and openSEF.

**Every detail matters.**
**Every animation counts.**
**Every API must be clean.**
**Every component must integrate.**

This document serves as the canonical reference for:

- What VitusOS Ares is
- How openSEF works
- Why decisions were made
- Where we're heading

**For developers:** Use this as your guide when implementing features.
**For contributors:** Use this to understand the vision before changing anything.
**For AI assistants:** READ THIS COMPLETELY before suggesting any changes.

**The goal is not to build another Linux DE.**
**The goal is to build the Linux desktop macOS users wish they had.**

---

**VitusOS Ares** - Reaching for Mars, Building the Future

*Document Version: 1.0*
*Last Updated: January 15, 2026*
*Author: @raven1zed (Human Architect) with Claude (Documentation AI)*
