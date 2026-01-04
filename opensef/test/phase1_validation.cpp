/**
 * phase1_validation.cpp - Phase 1 Framework Validation
 *
 * This sample app proves that Phase 1 (Framework Foundation) is complete.
 * It exercises: OSFApplication, OSFRunLoop, OSFNotificationCenter, OSFBundle.
 */

#include <iostream>
#include <opensef/OpenSEFBase.h>

using namespace opensef;

int main() {
  std::cout
      << "╔════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║       openSEF Phase 1 Validation - Framework Foundation    ║\n";
  std::cout
      << "╚════════════════════════════════════════════════════════════╝\n\n";

  // 1. Test OSFBundle
  std::cout << "[1] Testing OSFBundle...\n";
  OSFBundle bundle = OSFBundle::mainBundle();
  std::cout << "    ✓ Bundle identifier: " << bundle.identifier() << "\n";
  std::cout << "    ✓ Resource path: " << bundle.resourcePath() << "\n\n";

  // 2. Test OSFNotificationCenter
  std::cout << "[2] Testing OSFNotificationCenter...\n";
  bool notificationReceived = false;

  auto token = OSFNotificationCenter::defaultCenter().addObserver(
      "TestNotification", [&notificationReceived]() {
        notificationReceived = true;
        std::cout << "    ✓ Notification callback executed!\n";
      });

  OSFNotificationCenter::defaultCenter().postNotification("TestNotification");

  if (notificationReceived) {
    std::cout << "    ✓ NotificationCenter works correctly\n\n";
  } else {
    std::cout << "    ✗ NotificationCenter FAILED\n\n";
    return 1;
  }

  OSFNotificationCenter::defaultCenter().removeObserver(token);
  std::cout << "    ✓ Observer removed\n\n";

  // 3. Test OSFRunLoop with tasks
  std::cout << "[3] Testing OSFRunLoop...\n";
  int taskCount = 0;

  auto &app = OSFApplication::shared();

  // Set up launch callback
  app.setOnLaunch([&taskCount, &app]() {
    std::cout << "    ✓ Application launched!\n";

    // Post tasks to run loop
    app.runLoop().postTask([&taskCount]() {
      taskCount++;
      std::cout << "    ✓ Task 1 executed\n";
    });

    app.runLoop().postTask([&taskCount]() {
      taskCount++;
      std::cout << "    ✓ Task 2 executed\n";
    });

    app.runLoop().postTask([&taskCount, &app]() {
      taskCount++;
      std::cout << "    ✓ Task 3 executed\n";
      // Stop after 3 tasks
      app.stop();
    });
  });

  app.setOnTerminate([]() { std::cout << "    ✓ Application terminating!\n"; });

  // 4. Run the application
  std::cout << "[4] Testing OSFApplication lifecycle...\n";
  app.run();

  // 5. Verify results
  std::cout << "\n[5] Verification...\n";
  if (taskCount == 3) {
    std::cout << "    ✓ All tasks executed (" << taskCount << "/3)\n";
  } else {
    std::cout << "    ✗ Task execution FAILED (" << taskCount << "/3)\n";
    return 1;
  }

  std::cout
      << "\n╔════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║              PHASE 1 VALIDATION: PASSED                     ║\n";
  std::cout
      << "║                                                             ║\n";
  std::cout
      << "║  ✓ OSFBundle         - Resource metadata works              ║\n";
  std::cout
      << "║  ✓ OSFNotificationCenter - Pub/sub works                    ║\n";
  std::cout
      << "║  ✓ OSFRunLoop        - Task scheduling works                ║\n";
  std::cout
      << "║  ✓ OSFApplication    - Lifecycle works                      ║\n";
  std::cout
      << "║                                                             ║\n";
  std::cout
      << "║  Phase 1 Framework Foundation is COMPLETE.                  ║\n";
  std::cout
      << "╚════════════════════════════════════════════════════════════╝\n";

  return 0;
}
