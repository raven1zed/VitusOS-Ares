# openSEF API Reference

**Version:** 0.1.0  
**Theme:** Ares (The Martian)

---

## Core Classes

### OSFObject (OpenSEFBase.h)
Root class for all openSEF objects.

```objc
@interface OSFObject : NSObject
@property (readonly) NSString *objectID;
+ (instancetype)create;
@end
```

---

## UI Widgets

### OSFWindow
Top-level window.

```objc
OSFWindow *win = [OSFWindow windowWithTitle:@"My App" 
                                      frame:CGRectMake(0, 0, 800, 600)];
[win show];
```

### OSFView
Base class for all visual elements.

```objc
@property CGRect frame;
@property CGFloat alpha;
@property BOOL hidden;
- (void)addSubview:(OSFView *)view;
```

### OSFButton
Clickable button.

```objc
OSFButton *btn = [OSFButton buttonWithLabel:@"Click Me" action:^{
    NSLog(@"Clicked!");
}];
```

### OSFTextField
Text input field.

```objc
OSFTextField *field = [[OSFTextField alloc] init];
field.placeholder = @"Enter text...";
field.onTextChanged = ^(NSString *text) { ... };
```

### OSFLabel
Text display.

```objc
OSFLabel *label = [[OSFLabel alloc] init];
label.text = @"Hello World";
label.font = [OSFTypography headlineMedium];
label.textColor = [OSFColors textPrimary];
```

### OSFGlassPanel
Frosted glass effect panel (Ares signature).

```objc
OSFGlassPanel *panel = [OSFGlassPanel glassPanelWithFrame:frame];
panel.blurRadius = [OSFStyle blurRadiusMedium];
panel.tintColor = [OSFColors surface];
```

### OSFProgressBar
Progress indicator.

```objc
OSFProgressBar *progress = [[OSFProgressBar alloc] init];
[progress setProgress:0.5 animated:YES];
```

### OSFSwitch
Toggle switch.

```objc
OSFSwitch *toggle = [[OSFSwitch alloc] init];
toggle.onToggle = ^(BOOL on) { ... };
```

### OSFSlider
Value slider.

```objc
OSFSlider *slider = [[OSFSlider alloc] init];
slider.minValue = 0;
slider.maxValue = 100;
slider.onValueChanged = ^(CGFloat value) { ... };
```

### OSFDropdown
Dropdown menu.

```objc
OSFDropdown *dropdown = [[OSFDropdown alloc] init];
dropdown.options = @[@"Option 1", @"Option 2"];
dropdown.onSelection = ^(NSInteger index) { ... };
```

---

## Design System

### OSFColors (Ares Palette)

| Method | Color | Hex |
|--------|-------|-----|
| `primary` | Space Orange | #E85D04 |
| `secondary` | Mission Blue | #3D5A80 |
| `background` | Lunar Gray | #F0F0F0 |
| `textPrimary` | Space Charcoal | #1A1A2E |

### OSFTypography

| Method | Size |
|--------|------|
| `displayLarge` | 57pt |
| `headlineMedium` | 28pt |
| `bodyMedium` | 14pt |
| `labelSmall` | 11pt |

### OSFSpacing

| Method | Value |
|--------|-------|
| `xs` | 4pt |
| `sm` | 8pt |
| `md` | 16pt |
| `lg` | 24pt |

### OSFStyle

| Method | Value |
|--------|-------|
| `cornerRadiusMedium` | 8pt |
| `blurRadiusMedium` | 16pt |
| `shadowBlur` | 12pt |

---

## Backend

### OSFBackend
Display server connection.

```objc
OSFBackend *backend = [OSFBackend sharedBackend];
[backend connect];
[backend run];  // Event loop
[backend disconnect];
```

### OSFVulkanRenderer
Vulkan rendering.

```objc
OSFVulkanRenderer *renderer = [OSFVulkanRenderer sharedRenderer];
[renderer initialize];
[renderer beginFrame];
[renderer drawBlurWithRadius:16 inRect:rect];
[renderer endFrame];
```
