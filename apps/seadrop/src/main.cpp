/**
 * main.cpp - SeaDrop entry point
 */

#include "SeaDrop.h"

int main(int argc, char **argv) {
  vitusos::seadrop::SeaDropApp app;
  return app.run(argc, argv);
}
