# VitusOS Ares - Boot to Shutdown Integration Plan

## The Goal

**Seamless compositor integration from boot to shutdown, working effortlessly across all processes.**

---

## Current Status

### What Works ✅
- Compositor runs manually via `run_vitus_ares.sh`
- Desktop displays in WSLg (just like GNOME/KDE)
- Panel, dock, wallpaper all render
- Windows can be created

### What's Missing ❌
- No automatic boot integration
- No boot animation (shows systemd logs)
- No automatic lockscreen on boot
- No clean shutdown sequence
- Compositor not integrated with unified framework

---

## The Complete Flow

### 1. Boot Sequence

**What Users Should See:**
```
Power On
    ↓
Beautiful VitusOS Boot Animation
    ↓
Lockscreen
```

**What Actually Happens:**
```
1. Bootloader → Kernel
2. systemd starts services
3. Display manager starts (or auto-login)
4. VitusOS compositor launches
5. Boot animation service displays graphics
6. Lockscreen appears when ready
```

**NO systemd logs visible. Just smooth graphics.**

### 2. Compositor Startup

**Current** (Manual):
```bash
./run_vitus_ares.sh
```

**Target** (Automatic):
```bash
# systemd service or display manager integration
systemctl start vitusos-compositor
```

**What Compositor Does:**
1. Initialize wlroots backend
2. Create output (display)
3. Start unified framework (`OSFDesktop::shared()->initialize()`)
4. Launch shell components (panel, dock, wallpaper)
5. Show lockscreen or desktop

### 3. Desktop Running

**Compositor manages:**
- Window rendering
- Input handling
- Output management
- **Registers windows with framework** ← NEW

**Shell provides:**
- Panel (global menu, system tray, clock)
- Dock (running apps)
- Wallpaper

**Framework coordinates:**
- Window state
- Events
- Services
- Resources
- Theme

### 4. Shutdown Sequence

**What Users Should See:**
```
Click Shutdown
    ↓
Smooth Fade Out Animation
    ↓
Screen Goes Black
    ↓
Power Off
```

**What Actually Happens:**
```
1. User clicks shutdown in menu
2. Framework publishes "shutdown.requested" event
3. Applications save state and exit
4. Shell components stop
5. Compositor shows shutdown animation
6. Compositor exits cleanly
7. systemd powers off
```

**NO "Stopping service..." messages. Just smooth fade.**

---

## WSL2/WSLg Integration

### How It Works

**WSLg Architecture:**
```
Windows (Host)
    ↓
WSLg (Weston compositor + RDP)
    ↓
WSL2 (Linux)
    ↓
VitusOS Compositor (Wayland nested)
```

**When you run VitusOS on WSL2:**
1. VitusOS compositor starts as Wayland client
2. Connects to WSLg's Weston compositor
3. WSLg forwards to Windows via RDP
4. Windows shows VitusOS desktop in a window

**Just like GNOME/KDE on WSL2!** ✅

### Current WSLg Status

**Working:**
- ✅ Compositor displays in Windows
- ✅ Panel and dock visible
- ✅ Wallpaper renders
- ✅ Windows can be created
- ✅ Input events work

**Issues:**
- ⚠️ Dropdown menus glitch (layer shell rendering)
- ❌ Lockscreen not visible (OVERLAY layer issue)

**Root Cause**: WSLg's RDP-Rail has quirks with layer shell surfaces.

**Solution**: Implement proper XDG popups (already planned).

---

## Integration Roadmap

### Phase 1: Framework Integration (Current)
- [x] Create unified framework
- [x] Build successfully
- [ ] Integrate compositor with framework
- [ ] Integrate shell with framework

### Phase 2: Boot Integration
- [ ] Create systemd service
- [ ] Implement boot animation
- [ ] Auto-start compositor on boot
- [ ] Launch lockscreen automatically

### Phase 3: Shutdown Integration
- [ ] Implement shutdown animation
- [ ] Clean compositor exit
- [ ] Save application state
- [ ] Graceful service shutdown

### Phase 4: WSLg Optimization
- [ ] Fix dropdown rendering (XDG popups)
- [ ] Fix lockscreen visibility
- [ ] Optimize performance
- [ ] Test all features

---

## Compositor Integration with Framework

### Current Compositor (Pure C)

```c
// compositor/src/main.c
int main() {
  struct osf_server server;
  osf_server_init(&server);
  
  // Run compositor
  wl_display_run(server.wl_display);
  
  return 0;
}
```

### Integrated Compositor (C + Framework)

```c
// compositor/src/main.c
#include <OSFDesktop.h>

int main() {
  // Initialize framework
  OSFDesktop_init();
  
  struct osf_server server;
  osf_server_init(&server);
  
  // Run compositor
  wl_display_run(server.wl_display);
  
  // Cleanup
  OSFDesktop_terminate();
  
  return 0;
}
```

### Window Registration

```c
// compositor/src/view.c
void osf_new_xdg_toplevel(...) {
  struct osf_view *view = calloc(1, sizeof(*view));
  
  // ... setup view ...
  
  // Register with framework
  OSFWindow *window = OSFWindow_create(
    view->id,
    view->title,
    view->app_id
  );
  OSFWindowManager_registerWindow(window);
  
  // Framework automatically:
  // - Adds to state manager
  // - Publishes "window.created" event
  // - Notifies panel/dock
}
```

---

## Boot Animation Implementation

### Service File

```ini
# /etc/systemd/system/vitusos-boot-animation.service
[Unit]
Description=VitusOS Boot Animation
Before=vitusos-compositor.service

[Service]
Type=oneshot
ExecStart=/usr/bin/vitusos-boot-animation
RemainAfterExit=yes

[Install]
WantedBy=graphical.target
```

### Boot Animation Program

```cpp
// apps/boot-animation/main.cpp
int main() {
  // Connect to early compositor or framebuffer
  auto* display = connectToDisplay();
  
  // Show Mars-themed boot animation
  showBootAnimation(display);
  
  // Wait for compositor to be ready
  waitForCompositor();
  
  // Fade to lockscreen
  fadeOut();
  
  return 0;
}
```

---

## Compositor Service

### Service File

```ini
# /etc/systemd/system/vitusos-compositor.service
[Unit]
Description=VitusOS Ares Compositor
After=vitusos-boot-animation.service

[Service]
Type=simple
ExecStart=/usr/bin/opensef-compositor
Restart=on-failure
Environment=XDG_RUNTIME_DIR=/run/user/%U
Environment=WLR_RENDERER=pixman

[Install]
WantedBy=graphical.target
```

---

## The Complete Experience

### On Native Linux

```
1. GRUB bootloader
2. Kernel loads
3. systemd starts
4. Boot animation shows (hides systemd)
5. Compositor starts
6. Lockscreen appears
7. User logs in
8. Desktop loads
9. User works
10. User clicks shutdown
11. Shutdown animation plays
12. System powers off
```

**User sees**: Beautiful graphics from boot to shutdown  
**User doesn't see**: systemd, logs, terminals, technical details

### On WSL2

```
1. wsl -d VitusOS
2. Compositor starts
3. WSLg shows desktop in Windows
4. User works
5. User exits
6. Compositor stops
```

**Just like GNOME/KDE on WSL2!**

---

## Next Steps

### Immediate (This Session)
1. Integrate compositor with framework
2. Test window registration
3. Verify events are published

### Short Term (Next Session)
1. Refactor shell to use framework
2. Fix dropdown menus (XDG popups)
3. Test complete integration

### Medium Term (Future)
1. Implement boot animation
2. Create systemd services
3. Test boot-to-shutdown flow

### Long Term (Production)
1. Package for distribution
2. Create installer
3. Documentation for users

---

## Success Criteria

### For Compositor
- ✅ Starts automatically on boot (or manual for WSL2)
- ✅ Integrates with unified framework
- ✅ Registers windows automatically
- ✅ Publishes events correctly
- ✅ Works seamlessly in WSLg

### For Desktop Experience
- ✅ Boot animation (no systemd logs)
- ✅ Smooth lockscreen transition
- ✅ Integrated desktop (panel, dock, wallpaper)
- ✅ Automatic window management
- ✅ Clean shutdown animation

### For WSL2
- ✅ Displays in Windows (like GNOME/KDE)
- ✅ All features work
- ✅ Good performance
- ✅ Easy to start/stop

---

## You're Absolutely Right!

**Yes**, we need the compositor working seamlessly from boot to shutdown.

**Yes**, WSL2 should show the display with WSLg, just like GNOME/KDE.

**And we're building exactly that!**

The foundation (unified framework) is ready. Now we integrate the compositor, and everything will work together seamlessly.

---

**Let's make this happen!**
