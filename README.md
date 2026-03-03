# RainDropOS

RainDropOS is a custom Linux distribution designed for a **console-style PC gaming experience**. It is **Arch Linux–based** (not a custom kernel) and ships a **controller-first, full-screen shell** written in modern **C++20**.

The goal: boot straight into a minimal, game-focused UI that launches games reliably, with sensible defaults and performance-minded tuning.

## What RainDropOS Is

- **Arch-based distro** built with a reproducible ISO pipeline (planned via `archiso`)
- **Gaming stack** built around the open ecosystem:
  - Mesa / Vulkan
  - Steam + Proton (planned integration)
- **Controller-first UI shell** (C++20) that acts like a console home screen

## Goals

RainDropOS aims to deliver:

- A **boot-to-UI** gaming environment (no desktop required)
- A clean **game library + launcher** experience from a couch/controller
- **Steam + Proton** support for Windows titles (planned)
- **Performance profiles** per game (planned): CPU governor, GPU power limits, resolution scaling presets, etc.
- Built to be **hackable**: clear structure, open tooling, and documentation

---

## Project Status

This project is in active development. Expect missing features, breaking changes, and the occasional bout of optimism.

---

## Repository Layout

- `iso/`  
  ISO build scripts and configuration (ArchISO pipeline)
- `packages/`  
  PKGBUILDs and packaging resources for RainDropOS components
- `src/shell/`  
  The RainDropOS full-screen shell (C++20, controller-first UI)
- `src/daemons/`  
  Background services (game detection, profiles, telemetry, launch helpers)
- `games/`  
  Optional user-provided content (no proprietary assets included by default)
- `docs/`  
  Design notes, planning, technical documentation

---

## Roadmap (High-Level)

### 1) Base System
- Reproducible Arch-based ISO build
- Boot validation on a reference machine
- Auto-launch shell on boot

### 2) Shell Prototype
- Controller navigation
- Library UI (basic listing)
- Launch and return cleanly after game exit

### 3) Steam + Proton
- Install and configure Steam + Proton
- Detect Steam library and launch titles
- Basic options for Proton version + launch args

### 4) Performance & Telemetry
- Daemon to apply per-game profiles
- Telemetry collection: FPS, CPU/GPU usage, thermals
- UI overlay or shell panel to display metrics

### 5) Installer + Updates
- User-friendly installer (e.g., Calamares)
- Package repo + update mechanism
- Installation and recovery docs

### 6) Hardware Coverage + UX Polish
- Expand device compatibility (GPUs, laptops, handheld-style PCs)
- UI polish: animations, theming, accessibility
- Recommended open-source game list (installable, not bundled)

---

## Milestones

1. **Boot + Shell Demo**  
   ISO boots, auto-launches shell, runs a user-installed game, returns cleanly to UI.
2. **Shell Prototype**  
   Controller navigation + library display + reliable launching.
3. **Steam Integration**  
   Steam/Proton detected and usable from the shell.
4. **Performance Manager**  
   Profiles + telemetry available to the shell/overlay.
5. **Installer + Updates**  
   Bare-metal install + upgrade path.
6. **Polish + Wider Hardware Support**  
   Refined UX and broader compatibility testing.

---

## License

RainDropOS is released under the **Apache License 2.0**. See `LICENSE` for details.
