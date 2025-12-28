/**
 * hello-window.m - openSEF Hello World test
 *
 * Creates a simple window to validate the framework builds and runs.
 */

#import <opensef/opensef.h>

int main(int argc, char *argv[]) {
  @autoreleasepool {
    NSLog(@"[openSEF] Hello Window Test");
    NSLog(@"[openSEF] Version: %s", OPENSEF_VERSION_STRING);

    // Connect to Wayland
    OSFBackend *backend = [OSFBackend sharedBackend];
    if (![backend connect]) {
      NSLog(@"[openSEF] ERROR: Failed to connect to Wayland");
      return 1;
    }

    // Create a simple window
    OSFWindow *window =
        [OSFWindow windowWithTitle:@"Hello VitusOS"
                             frame:CGRectMake(100, 100, 400, 300)];

    // Add glass background
    OSFGlassPanel *glass =
        [OSFGlassPanel glassPanelWithFrame:CGRectMake(0, 0, 400, 300)];
    glass.blurRadius = [OSFStyle blurRadiusMedium];
    [window setContentView:glass];

    // Add centered label
    OSFLabel *label = [[OSFLabel alloc] init];
    label.frame = CGRectMake(50, 120, 300, 60);
    label.text = @"Welcome to VitusOS";
    label.font = [OSFTypography headlineMedium];
    label.textColor = [OSFColors textPrimary];
    [glass addSubview:label];

    // Add subtitle
    OSFLabel *subtitle = [[OSFLabel alloc] init];
    subtitle.frame = CGRectMake(50, 170, 300, 30);
    subtitle.text = @"openSEF Framework is working!";
    subtitle.font = [OSFTypography bodyMedium];
    subtitle.textColor = [OSFColors textSecondary];
    [glass addSubview:subtitle];

    // Show window
    [window show];

    NSLog(@"[openSEF] Window created. Running event loop...");

    // Run event loop (blocks until window closed)
    [backend run];

    // Cleanup
    [backend disconnect];

    NSLog(@"[openSEF] Goodbye!");
    return 0;
  }
}
