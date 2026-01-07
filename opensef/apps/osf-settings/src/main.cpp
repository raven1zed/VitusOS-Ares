#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <iostream>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBase.h>

using namespace opensef;

int main(int argc, char **argv) {
  auto &app = OSFApplication::shared();
  app.setAppID("osf-settings");

  // Hook into lifecycle events
  app.setOnLaunch([&]() {
    std::cout << "[osf-settings] Launching..." << std::endl;

    auto *desktop = OpenSEF::OSFDesktop::shared();
    OpenSEF::OSFEvent event;
    event.set("app_id", app.appID());
    event.set("name", "Settings");
    desktop->eventBus()->publish(OpenSEF::OSFEventBus::APP_LAUNCHED, event);
  });

  app.setOnTerminate([&]() {
    std::cout << "[osf-settings] Terminating..." << std::endl;

    auto *desktop = OpenSEF::OSFDesktop::shared();
    OpenSEF::OSFEvent event;
    event.set("app_id", app.appID());
    desktop->eventBus()->publish(OpenSEF::OSFEventBus::APP_TERMINATED, event);
  });

  // Create Main Window
  auto window = std::make_shared<OSFWindow>(600, 400, "Settings");
  window->onClose([&]() { app.stop(); });
  window->show();

  // Run Event Loop
  app.run();
  return 0;
}
