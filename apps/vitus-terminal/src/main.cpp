/**
 * VitusOS Terminal - Entry point
 */

#include "Terminal.h"

int main(int argc, char **argv) {
  vitusos::terminal::TerminalApp app;
  return app.run(argc, argv);
}
