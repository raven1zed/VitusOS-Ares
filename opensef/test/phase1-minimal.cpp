/**
 * phase1-minimal.cpp - Phase 1 minimal app scaffolding
 *
 * Demonstrates OSFApplication + OSFWindow + OSFView wiring.
 */

#include <iostream>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBase.h>


using namespace opensef;

int main() {
  std::cout << "[openSEF] Phase 1 minimal app starting..." << std::endl;

  auto &app = OSFApplication::shared();

  app.setOnLaunch([&app]() {
    auto window = OSFWindow::create("Phase 1 Minimal App", OSFRect(0, 0, 480, 320));
    auto rootView = std::make_shared<OSFView>();
    rootView->setFrame(OSFRect(0, 0, 480, 320));

    auto label = OSFLabel::createTitle("Phase 1 Foundation Ready");
    label->setFrame(OSFRect(24, 24, 432, 32));
    rootView->addSubview(label);
    window->setContentView(rootView);
    window->show();

    app.runLoop().postTask([&app]() { app.stop(); });
  });

  app.setOnTerminate([]() {
    std::cout << "[openSEF] Phase 1 minimal app terminated." << std::endl;
  });

  app.run();
  return 0;
}
