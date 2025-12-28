/**
 * VitusOS Filer - Entry point
 */

#include "Filer.h"

int main(int argc, char **argv) {
  vitusos::filer::FilerApp app;
  return app.run(argc, argv);
}
