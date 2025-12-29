openSEF Design Reference

**VitusOS Ares Desktop Environment**

*Reference document for Claude Opus implementation*

# **Project Context**
VitusOS Ares is a unified Linux desktop experience combining:

- OS1 minimalism (Her, 2013) - Technology that recedes into background
- macOS Aqua vitality - Polished, animated, delightful interface
- Ares theme - The Martian (2015) inspired aesthetic

## **Core Mission**
"Bringing Life to the Beautiful Future We Were Promised"
## **Framework**
openSEF (Custom GUI framework, C++/ObjC, Wayland + Vulkan)
## **Design Philosophy**
- Warm over cold
- Minimal over complex
- Integrated over fragmented
- Beautiful AND functional
# **Design System: Ares Theme**
## **Color Palette**
### **Primary (Mars Mission)**

|**Color**|**Code**|
| :- | :- |
|**Space Orange**|#E85D04|
|**Mars Dust**|#FB8500|
|**Mission Blue**|#3D5A80|
|**Lunar Gray**|#F0F0F0|
### **OS1-Inspired Warmth**

|**Color**|**Code**|
| :- | :- |
|**Deep Burgundy**|#621212|
|**Warm Gold**|#C3BC19|
|**Cool Teal**|#389693|
|**Soft Cream**|#FBFBFB|
|**Lavender Mist**|#EFEBF7|
### **Text & UI**

|**Element**|**Code**|
| :- | :- |
|**Text Primary**|#2D2D2D|
|**Text Secondary**|#6B6B6B|
|**Text Tertiary**|#9E9E9E|
|**Borders**|#E0DDD9|
## **Typography**
### **Font Stack**
- Primary: Inter, SF Pro Text, Segoe UI, system-ui
- Monospace: JetBrains Mono, SF Mono, Cascadia Code
### **Hierarchy**
- Display: 24-32px, Medium weight
- Heading: 18-20px, Medium weight
- Body: 14px, Regular weight
- Small: 12px, Regular weight
### **Characteristics**
- High legibility sans-serif
- Medium weight as default (not light, not bold)
- Generous line spacing (1.5-1.6)
- Anti-aliased with subpixel rendering


# **Reference 1: OS1 from Her (2013)**
## **Visual References**
### **Screenshot Galleries**
- https://shot.cafe/movie/her-2013/computer+screen
- https://screenmusings.org/movie/blu-ray/Her/
- https://film-grab.com/2014/05/06/her/
- https://kissthemgoodbye.net/movie/thumbnails.php?album=146
### **Analysis**
- https://scifiinterfaces.com/category/her-2013/
- https://www.pushing-pixels.org/2018/04/05/screen-graphics-of-her-interview-with-geoff-mcfetridge.html
## **Key Visual Elements**
### **1. Color Philosophy**
- Warm dominant: Reds, pinks, creams, muted oranges
- Cool accents: Blues/greens used sparingly for contrast
- No harsh whites: Always cream/off-white (#FBFBFB, #EFEBF7)
- Comfortable, inviting: Technology feels human, not clinical
### **2. Interface Components**
- Minimal chrome: Window borders barely visible
- Traditional elements simplified: Windows, menus exist but understated
- No keyboards visible: Voice-first interaction
- Large display focus: Content fills screen, UI recedes
- Context-adaptive: Interface changes per user/context
### **3. Typography & Layout**
- Clean sans-serif: Readable, not futuristic
- Generous whitespace: Not cramming information
- Content-first hierarchy: UI chrome secondary to content
- Subtle depth: Gentle shadows, not flat but not skeuomorphic
### **4. Interaction Design**
- Smooth, organic animations: Flowing, natural
- Gentle feedback: No harsh clicks or jarring transitions
- Invisible interaction: Technology that doesn't look like technology
- "Undesigned design": Interface disappears into experience
## **Elements to Borrow**
- Warm color palette (reds, pinks, creams)
- Minimal window chrome (subtle borders, thin title bars)
- Generous negative space (breathing room in layouts)
- Soft shadows (depth without harshness)
- Rounded corners (gentle, not extreme)
- Smooth transitions (200-300ms, easing functions)
- Content-first layouts (UI recedes)
- Organic motion (flowing, alive feeling)


# **Reference 2: macOS Aqua/Cocoa**
## **Visual References**
### **Screenshot Library**
- https://512pixels.net/projects/aqua-screenshot-library/
### **Documentation**
- Apple Aqua Human Interface Guidelines (Classic)
- https://www.stone.com/The\_Cocoa\_Files/Arise\_Aqua\_.html
## **Key Visual Elements**
### **1. Aqua Appearance**
- Anti-aliased everything: Text, graphics, icons
- Subtle transparency: Title bars, panels
- Careful color use: Not garish, refined palette
- Shadows for depth: Windows float above desktop
- Animated effects: Minimize/maximize genie effect
### **2. Window System**
- Three-button traffic lights: Close (red), minimize (yellow), maximize (green)
- Unified title bar: Title + toolbar combined
- Drawers: Side panels that slide out
- Sheets: Modal dialogs drop from title bar
- Single window mode: Reduces clutter
### **3. Controls & Widgets**
- "Gum drop" buttons: Pills, rounded rectangles
- Visual states: Default, hover, active, disabled
- Consistent sizing: Standard heights (22px, 28px, 32px)
- Aqua/Graphite themes: Blue accent vs. gray accent


# **Reference 3: GhostBSD Gershwin**
## **Visual References**
### **Demo & Resources**
- https://www.youtube.com/watch?v=0F5btud6Zjc (Testing out GhostBSD's Gershwin)
- https://github.com/gershwin-desktop/gershwin-desktop
- https://itsfoss.com/news/ghostbsd-launches-gershwin/
## **Key Implementation Insights**
### **1. Technical Architecture**
- GNUstep base: GWorkspace (file manager, desktop, dock)
- Window manager: Xfce4-WM (custom WM planned)
- Global menu: Xfce panel provides Mac-style menu bar
- Hybrid compatibility: Works with GNUstep + GTK apps seamlessly
### **2. Visual Approach**
- Early Mac OS X aesthetic: Classic Aqua look
- Global menu bar: Always at top of screen
- Dock-based launching: App management through dock
- Minimal desktop clutter: Files managed through file manager


# **Window System Specification**
## **Window Chrome**
### **Title Bar**

|**Property**|**Value**|
| :- | :- |
|**Height**|32px (standard), 28px (compact)|
|**Background**|Lunar gray (#F0F0F0) with 90% opacity|
|**Active**|Space orange accent on left edge (3px)|
|**Inactive**|No accent, slightly more transparent|
### **Traffic Lights (macOS-inspired)**
- Close: Space Orange (#E85D04)
- Minimize: Warm Gold (#C3BC19)
- Maximize: Mission Blue (#3D5A80)
- Size: 12px diameter, 8px spacing
- Position: 8px from left, vertically centered


# **Widget Specifications**
## **Buttons**
### **Default Button**

|**Property**|**Value**|
| :- | :- |
|**Background**|#FBFBFB (soft cream)|
|**Border**|1px solid #E0DDD9|
|**Text**|#2D2D2D, 14px Regular|
|**Height**|28px|
|**Padding**|0 16px|
|**Border-radius**|6px|
### **Primary Button (Aqua-inspired)**

|**Property**|**Value**|
| :- | :- |
|**Background**|Space Orange (#E85D04)|
|**Border**|None|
|**Text**|White, 14px Medium|
|**Height**|28px|
|**Padding**|0 16px|
|**Border-radius**|6px|
|**Shadow**|0 1px 3px rgba(229, 93, 4, 0.3)|


# **Animation Specifications**
## **Timing Functions**
### **Standard Easing**
- ease-out: cubic-bezier(0.0, 0.0, 0.2, 1) – Decelerating
- ease-in-out: cubic-bezier(0.4, 0.0, 0.2, 1) – Smooth both ends
### **Duration Standards**
- Instant: 100ms (button press, hover)
- Quick: 200ms (menu open, tooltip)
- Standard: 300ms (window minimize, drawer slide)
- Slow: 500ms (workspace switch, major transitions)
## **Window Animations**
### **Minimize (Genie effect, simplified)**
- Duration: 300ms ease-in-out
- Scale down to dock position
- Fade opacity 1.0 → 0.0
- Slight stretch toward dock (elastic feel)
### **Maximize**
- Duration: 200ms ease-out
- Scale from current size to full screen
- Maintain corner origin
### **Close**
- Duration: 200ms ease-in
- Scale down slightly (0.95) + fade out
- Maintain center position


# **Implementation Notes for Opus**
## **Technical Stack**
- Framework: openSEF (C++/ObjC)
- opensef-base: Foundation classes
- opensef-appkit: GUI widgets
- opensef-backend: Wayland + Vulkan rendering
- opensef-ui: Ares design system implementation
## **Priority Implementation Order**
### **Phase 1: Core Window System**
1. Basic window with title bar + traffic lights
1. Window shadows and borders
1. Window animations (minimize, maximize, close)
1. Focus management (active/inactive states)
### **Phase 2: Widget Library**
1. Buttons (default, primary, states)
1. Text fields (input, focus states)
1. Labels and text rendering
1. Sliders, checkboxes, radio buttons
### **Phase 3: Desktop Components**
1. Global menu bar (top panel)
1. Dock (bottom launcher with magnification)
1. Workspace/desktop manager
1. System tray icons
### **Phase 4: Polish**
1. All animations functioning
1. Consistent theming across widgets
1. Icon set complete
1. GTK app integration (global menu)


# **Design Philosophy Summary**
## **The Ares Experience**
### **User sees VitusOS and feels:**
1. Warmth – Not cold corporate tech (OS1 + Ares colors)
1. Polish – Not rough/unfinished (Aqua vitality)
1. Simplicity – Not overwhelming complexity (OS1 minimalism)
1. Consistency – Not fragmented Linux chaos (unified design)
1. Purpose – "Science the shit out of this" determination (Ares mission)

### **The interface:**
- Recedes when not needed (OS1)
- Delights when interacted with (Aqua)
- Feels warm and human (Ares palette)
- Works seamlessly across apps (Gershwin lessons)
- Makes technology feel like it was designed for humans, by humans who care
## **"Bringing Life to the Beautiful Future We Were Promised"**
This isn't just another Linux desktop. This is the vision of computing that feels warm, not cold; technology that disappears into experience; interfaces that delight, not frustrate; systems that work together, not fragment; and design that respects users, not confuses them.

*That's VitusOS Ares. That's what we're building.*

Document prepared for Claude Opus implementation | VitusOS Ares Project | December 30, 2025
