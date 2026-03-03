# RainDropOS

RainDropOS is a custom Linux distribution designed for a **console-style PC gaming experience**. It is **Arch Linux–based** (not a custom kernel) and ships a **controller-first, full-screen shell** written in modern **C++20**.

The goal: boot straight into a minimal, game-focused UI that launches games reliably, with sensible defaults and performance-minded tuning.

---

## What RainDropOS Is

- **Arch-based distro** built with a reproducible ISO pipeline (via `archiso`)
- **Gaming stack** built around the open ecosystem:
  - Mesa / Vulkan
  - Steam + Proton (planned)
- **Controller-first UI shell** (C++20) that acts like a console home screen — no desktop, no taskbar, just your games

---

## Current State

The shell is actively being built. Here is what exists and works today:

**Shell (C++20 / SDL2)**
- Full render loop running at ~60fps
- Controller and keyboard input abstraction
- Screen stack with push/pop navigation
- Game library screen with navigable tile grid
- Game detail screen (title, cover art placeholder, launch/back options)
- Settings screen (fullscreen, resolution, vsync, exit)
- JSON-driven game library (`library.json`) — add games without recompiling
- Game launcher via `fork`/`execvp`/`waitpid` with clean return to shell
- Consistent UI: header bar, hint bar, border-based selection across all screens
- JetBrains Mono Nerd Font

**Infrastructure**
- CMake + Ninja build system
- C++20, Clang 18
- `nlohmann/json` for library parsing
- Bundled font assets

---

## Tech Stack

| Component | Technology |
|---|---|
| Shell language | C++20 |
| Windowing / input | SDL2 |
| Font rendering | SDL2_ttf |
| JSON parsing | nlohmann/json |
| Build system | CMake + Ninja |
| Compiler | Clang 18 |
| Base OS | Arch Linux |
| ISO tooling | archiso (planned) |

---

## Repository Layout

```
raindropos/
├── src/
│   ├── shell/          # Full-screen shell (C++20)
│   └── daemons/        # Background services (planned)
├── iso/                # ArchISO build pipeline (planned)
├── packages/           # PKGBUILDs for RainDropOS components (planned)
├── assets/
│   └── fonts/          # Bundled fonts
├── include/            # Third-party headers (nlohmann/json)
├── docs/               # Design notes and documentation
├── games/              # Optional user content
└── library.json        # Game library definition
```

---

## Building

### Prerequisites

- Linux (or WSL2 on Windows)
- `clang++`, `cmake`, `ninja`
- `libsdl2-dev`, `libsdl2-ttf-dev`

### Build

```bash
git clone https://github.com/ThatTanishqTak/raindropos.git
cd raindropos
cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
cmake --build build
./build/src/shell/raindrop-shell
```

### Adding Games

Edit `library.json` in the project root:

```json
{
    "games": [
        {
            "title": "My Game",
            "executable": "/usr/bin/my-game"
        }
    ]
}
```

No recompile needed — the shell reads this file on every launch.

---

## Controls

| Input | Action |
|---|---|
| Arrow Keys / D-Pad | Navigate |
| Enter / A | Confirm / Open |
| Escape / B | Back |
| Tab / Start | Settings |

---

## Roadmap

### ✅ Phase 0 — Infrastructure
- CMake + Ninja build pipeline
- C++20 toolchain (Clang 18)
- SDL2 + SDL2_ttf dependencies
- Git repository

### ✅ Phase 2 — Shell Prototype
- Render loop and input abstraction
- Screen stack navigation
- Game library with JSON backend
- Game launcher (fork/exec/wait)
- Settings screen
- Visual polish

### ⬜ Phase 1 — Base ISO
- Reproducible Arch-based ISO (archiso)
- Auto-launch shell on boot via systemd
- Boot validation in VirtualBox

### ⬜ Phase 3 — Steam + Proton
- Detect Steam libraries via VDF parsing
- Launch titles via Proton
- Per-game Proton version selection

### ⬜ Phase 4 — Performance Daemon
- Per-game CPU governor and GPU power profiles
- Telemetry: FPS, CPU/GPU usage, thermals
- IPC between shell and daemon via Unix socket

### ⬜ Phase 5 — Installer + Updates
- Calamares-based installer
- Custom Arch package repository
- In-shell update flow

### ⬜ Phase 6 — Polish + Hardware
- GPU compatibility (AMD, Intel, NVIDIA)
- On-screen keyboard
- Animations and theming system
- Recommended open-source game list

---

## Milestones

1. **Boot + Shell Demo** — ISO boots, shell auto-launches, game runs and returns cleanly *(in progress)*
2. **Shell Prototype** — Controller navigation, library, reliable launching ✅
3. **Steam Integration** — Steam/Proton detected and usable from the shell
4. **Performance Manager** — Profiles and telemetry in the shell
5. **Installer + Updates** — Bare-metal install and upgrade path
6. **Polish + Hardware** — Refined UX and broad compatibility

---

## License

RainDropOS is released under the **Apache License 2.0**. See `LICENSE` for details.