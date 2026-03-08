# RainDropOS

A minimal, Arch Linux-based operating system designed for a console-style PC gaming experience. RainDropOS boots directly into a controller-first shell — no desktop environment, no login prompt, just your game library.

> **Status:** Active development. Core shell and Steam integration are functional. Performance daemon in progress.

---

## Overview

RainDropOS replaces the traditional desktop with a full-screen C++20 shell that behaves like a console home screen. On boot, the system autologins and launches directly into the shell. Games are launched from a navigable library, run fullscreen, and return cleanly to the shell on exit.

Steam libraries are detected automatically. Manual entries can be added via a simple JSON file. A background daemon handles per-game performance profiles and telemetry collection.

---

## Features

**Shell**
- 60fps render loop with SDL2
- Controller and keyboard input with a unified action abstraction
- Screen stack navigation — library, game detail, settings
- Steam library auto-detection via VDF file parsing
- JSON-driven manual game library
- Game launcher via `fork` / `execvp` / `waitpid`
- Steam game launching via `steam://rungameid/<appid>`
- Consistent UI — header bar, hint bar, border-based selection

**Performance Daemon**
- Unix socket IPC with the shell
- Per-game CPU governor profiles
- Live telemetry — CPU usage, temperature, RAM
- Automatic profile restore on game exit

**System**
- Boots straight into the shell — no display manager, no desktop
- Autologin via getty override
- Tested on Arch Linux in VirtualBox

---

## Getting Started

### Prerequisites

- Linux or WSL2
- `clang++` `cmake` `ninja`
- `libsdl2-dev` `libsdl2-ttf-dev`

### Build

```bash
git clone https://github.com/ThatTanishqTak/raindropos.git
cd raindropos
cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

### Run

```bash
# Shell
./build/src/shell/raindrop-shell

# Daemon (optional, run alongside the shell)
./build/raindrop-daemon

# Tests
./build/vdf-test
./build/steam-test
```

---

## Configuration

### Game Library

Edit `library.json` in the project root to add games manually. The shell reads this file on every launch — no recompile required.

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

Steam games are detected automatically when Steam is installed and merged into the same library view.

### Performance Profiles

Create a per-game profile at `~/.config/raindrop/profiles/<appid>.json`:

```json
{
    "cpu_governor": "performance"
}
```

The daemon applies the profile on game launch and restores defaults on exit. If no profile exists for a game, system defaults are used.

---

## Controls

| Input | Action |
|---|---|
| Arrow Keys / D-Pad | Navigate |
| Enter / A | Confirm |
| Escape / B | Back |
| Tab / Start | Settings |

---

## Tech Stack

| Component | Technology |
|---|---|
| Shell | C++20 |
| Windowing / input | SDL2 |
| Font rendering | SDL2_ttf |
| JSON | nlohmann/json |
| IPC | Unix domain sockets |
| Build | CMake + Ninja |
| Compiler | Clang 18 |
| Base OS | Arch Linux |

---

## Roadmap

| Phase | Description | Status |
|---|---|---|
| 0 — Infrastructure | Build pipeline, toolchain, repo | ✅ Done |
| 1 — Base System | Arch install, autologin, boot to shell | ✅ Done |
| 2 — Shell | Render loop, input, library, launcher, settings | ✅ Done |
| 3 — Steam Integration | VDF parsing, library scan, Proton launch | ✅ Done |
| 4 — Performance Daemon | IPC, profiles, telemetry | 🔄 In progress |
| 5 — Installer | Calamares installer, package repo, updates | ⬜ Planned |
| 6 — Polish | GPU support, OSK, animations, hardware testing | ⬜ Planned |

---

## Contributing

This project is in active early development. Contributions, issues, and feedback are welcome. Please open an issue before submitting a large pull request.

---

## License

Released under the [Apache License 2.0](LICENSE).