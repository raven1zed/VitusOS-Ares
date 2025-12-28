/**
 * main.cpp - VitusOS Installer entry point
 */

#include "Installer.h"

int main(int argc, char **argv) {
  vitusos::installer::InstallerApp app;
  return app.run(argc, argv);
}
