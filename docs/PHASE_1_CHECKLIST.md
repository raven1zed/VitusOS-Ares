# Phase 1 Unified Checklist

This checklist merges the two Phase 1 definitions currently in the repo:

- **VitusOS Ares roadmap**: *Phase 1: Fork & Foundation (Weeks 1–8)* in `VitusOS Ares.md`
- **openSEF Design Reference**: *Phase 1: Core Window System* in `openSEF Design Reference.md`

Use it as a single, canonical Phase 1 checklist. Mark items complete as they land
in the repo, and add links to the relevant PRs or commits.

## 1) Fork & Foundation (roadmap track)

- [ ] Fork GNUstep repositories:
  - [ ] `gnustep/libs-base` → `opensef-base`
  - [ ] `gnustep/libs-gui` → `opensef-gui`
  - [ ] `gnustep/libs-back` → `opensef-backend`
- [ ] Rename project identifiers throughout the forked codebases:
  - [ ] Update references from GNUstep → openSEF
  - [ ] Update copyright headers / licensing
- [ ] Establish the new repo layout under the VitusOS org
- [ ] Analyze each fork for retention vs. replacement:
  - [ ] `opensef-base` (Foundation classes)
  - [ ] `opensef-gui` (AppKit/UI components)
  - [ ] `opensef-backend` (rendering backend)
- [ ] Define Wayland + GPU acceleration replacement plan for backend
- [ ] Migrate build system to CMake:
  - [ ] CMake project configured for openSEF
  - [ ] Core libs build from CMake (base/gui/backend)
  - [ ] Document build instructions

## 2) Core Window System (design-reference track)

- [ ] Basic window chrome:
  - [ ] Title bar
  - [ ] Traffic lights (close/minimize/maximize)
- [ ] Window shadows and borders
- [ ] Window animations:
  - [ ] Minimize
  - [ ] Maximize
  - [ ] Close
- [ ] Focus management:
  - [ ] Active window state
  - [ ] Inactive window state

## 3) Phase 1 Exit Criteria (unified)

- [ ] Forks exist and are renamed (base/gui/backend)
- [ ] CMake builds the core libraries
- [ ] A basic window can be created, focused, and animated
- [ ] Window chrome renders correctly (title bar + traffic lights)
- [ ] Shadows/borders are present in the core window system

## 4) Tracking Notes (optional)

- **Owner:** _(fill in)_
- **Target window:** _(fill in)_
- **Primary tracking issue:** _(fill in)_
