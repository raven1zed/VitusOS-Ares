# VitusOS Ares - AI Contributor Guide

This file provides working instructions for AI instances operating in this
repository. Follow these guidelines for all changes within this repo.

## Mission
VitusOS Ares (openSEF) is a hybrid Wayland desktop stack:
- **Pure C compositor** (`opensef/opensef-compositor`) built on wlroots.
- **C++ shell & UI** (`opensef/opensef-shell`, `opensef-appkit`, `opensef-core`, `opensef-base`)
  using Cairo/Pango.

Your task is to keep the architecture clean, the build predictable, and the
documentation accurate as the system evolves.

## Core Expectations
- **Respect the hybrid architecture:** compositor code stays in C; UI/framework
  code stays in C++.
- **Avoid design churn:** prioritize maintainability and minimal architectural
  disruption.
- **Keep docs aligned with reality:** if you change build or behavior, update the
  relevant docs.

## Build & Environment Notes
- Root `CMakeLists.txt` currently requires **Clang** for Objective‑C flags.
- The Nix shell (`flake.nix`) sets `CC`/`CXX`; align these if build issues arise.
- Shell clients are built from `opensef/opensef-shell` (panel, dock, wallpaper).
- The compositor is only built if `wlroots` is detected.

## How to Run (common dev flow)
```
nix develop
cmake -B build -G Ninja
cmake --build build
./scripts/start-opensef.sh
```

## Testing
- Phase validation binaries live under `opensef/test`.
- Use `ctest` if configured, otherwise run test binaries directly.

## Change Discipline
- Prefer small, focused commits.
- Preserve existing naming and code style per subsystem.
- If you touch build flags or dependencies, confirm related docs remain correct.
