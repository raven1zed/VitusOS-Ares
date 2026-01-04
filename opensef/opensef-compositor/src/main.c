/**
 * main.c - openSEF Compositor Entry Point
 *
 * Pure C Wayland compositor using wlroots.
 * VitusOS Ares Desktop Environment
 */

#define _POSIX_C_SOURCE 200112L

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <wlr/util/log.h>

#include "server.h"

static void print_usage(const char *name) {
  fprintf(stderr, "Usage: %s [options]\n", name);
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -s, --startup CMD    Run startup command\n");
  fprintf(stderr, "  -d, --debug          Enable debug logging\n");
  fprintf(stderr, "  -h, --help           Show this help\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "VitusOS Ares - openSEF Compositor\n");
}

static struct osf_server server = {0};

static void handle_signal(int sig) {
  (void)sig;
  wl_display_terminate(server.wl_display);
}

int main(int argc, char *argv[]) {
  char *startup_cmd = NULL;
  enum wlr_log_importance log_level = WLR_INFO;

  static struct option long_options[] = {
      {"startup", required_argument, NULL, 's'},
      {"debug", no_argument, NULL, 'd'},
      {"help", no_argument, NULL, 'h'},
      {NULL, 0, NULL, 0}};

  int opt;
  while ((opt = getopt_long(argc, argv, "s:dh", long_options, NULL)) != -1) {
    switch (opt) {
    case 's':
      startup_cmd = optarg;
      break;
    case 'd':
      log_level = WLR_DEBUG;
      break;
    case 'h':
      print_usage(argv[0]);
      return 0;
    default:
      print_usage(argv[0]);
      return 1;
    }
  }

  wlr_log_init(log_level, NULL);

  wlr_log(WLR_INFO, "╔════════════════════════════════════════╗");
  wlr_log(WLR_INFO, "║     openSEF Compositor                 ║");
  wlr_log(WLR_INFO, "║     VitusOS Ares Desktop               ║");
  wlr_log(WLR_INFO, "╚════════════════════════════════════════╝");

  /* Initialize server */
  if (!osf_server_init(&server)) {
    wlr_log(WLR_ERROR, "Failed to initialize compositor");
    return 1;
  }

  /* Set up signal handlers */
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  wlr_log(WLR_INFO, "Running on Wayland display: %s", server.socket);

  /* Set WAYLAND_DISPLAY for child processes */
  setenv("WAYLAND_DISPLAY", server.socket, true);

  /* Run startup command if provided */
  if (startup_cmd) {
    wlr_log(WLR_INFO, "Running startup command: %s", startup_cmd);
    if (fork() == 0) {
      execl("/bin/sh", "/bin/sh", "-c", startup_cmd, NULL);
      _exit(1);
    }
  }

  /* Run the compositor */
  osf_server_run(&server);

  /* Cleanup */
  osf_server_finish(&server);

  wlr_log(WLR_INFO, "Compositor shut down cleanly");
  return 0;
}
