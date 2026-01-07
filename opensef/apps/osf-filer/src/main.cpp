#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <iostream>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBase.h>


using namespace opensef;

int main(int argc, char **argv) {
  auto &app = OSFApplication::shared();
  app.setAppID("osf-filer");

  // Hook into lifecycle events to notify the system
  app.setOnLaunch([&]() {
    std::cout << "[osf-filer] Launching..." << std::endl;

    // Notify System (Dock/Shell)
    auto *desktop = OpenSEF::OSFDesktop::shared();
    OpenSEF::OSFEvent event;
    event.set("app_id", app.appID());
    event.set("name", "Filer");
    desktop->eventBus()->publish("application.launched", event);
  });

  app.setOnTerminate([&]() {
    std::cout << "[osf-filer] Terminating..." << std::endl;

    auto *desktop = OpenSEF::OSFDesktop::shared();
    OpenSEF::OSFEvent event;
    event.set("app_id", app.appID());
    desktop->eventBus()->publish(
        "application.closed", event); // Note: topic matches Dock subscription
  });

  // Create Main Window
  auto window = std::make_shared<OSFWindow>(900, 600, "Filer");
  window->onClose([&]() { app.stop(); });
  window->show();

  // Run Event Loop
  app.run();
  return 0;
}
