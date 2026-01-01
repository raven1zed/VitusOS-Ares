/**
 * OSFApplication.cpp - Application lifecycle
 */

#include <opensef/OpenSEFBase.h>


namespace opensef {

OSFApplication &OSFApplication::shared() {
  static OSFApplication app;
  return app;
}

void OSFApplication::run() {
  if (onLaunch_) {
    onLaunch_();
  }
  runLoop_.run();
  if (onTerminate_) {
    onTerminate_();
  }
}

void OSFApplication::stop() { runLoop_.stop(); }

} // namespace opensef
