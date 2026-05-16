# RainDropOS

A minimal, Arch Linux-based gaming operating system designed for a console-style PC experience. RainDropOS boots directly into a controller-first shell with no traditional desktop environment, no visible login flow, and no unnecessary desktop clutter between the player and their game library.

> **Status:** Active development. The core shell, Steam library detection, manual game library support, and basic game launching are functional. The performance daemon is in progress. Gamescope integration, SDL3 migration, systemd-based game session tracking, audio stack integration, installer tooling, and update/rollback support are planned.

---

## Overview

RainDropOS replaces the traditional Linux desktop with a full-screen C++ shell that behaves like a console home screen. On boot, the system autologins and launches directly into the RainDrop shell. Games are launched from a navigable library, run fullscreen, and return cleanly to the shell when they exit.

The current shell supports Steam library detection, manual JSON-based game entries, controller/keyboard navigation, and a basic launcher path. A background daemon is being developed to handle per-game performance profiles, telemetry collection, and future session-level system control.

The long-term goal is to evolve RainDropOS from a fullscreen shell prototype into a proper console-style Linux gaming environment using a dedicated game session layer, compositor support, reliable process tracking, audio integration, and rollback-safe system updates.

---

## Core Goals

- Boot directly into a controller-first gaming shell.
- Avoid a traditional desktop environment during normal use.
- Detect Steam games automatically and merge them with manual library entries.
- Launch native Linux, Steam, and Proton games cleanly.
- Apply per-game performance profiles through a background daemon.
- Track game sessions reliably, including child processes and launch wrappers.
- Provide a console-like user experience with predictable fullscreen behavior.
- Support safe system updates and rollback for appliance-style reliability.

---

## Current Features

### Shell

- 60 FPS render loop using SDL2.
- Controller and keyboard input through a unified action abstraction.
- Screen stack navigation for library, game detail, and settings views.
- Steam library auto-detection through VDF file parsing.
- JSON-driven manual game library.
- Native game launching through `fork`, `execvp`, and `waitpid`.
- Steam game launching through `steam://rungameid/<appid>`.
- Consistent UI structure with header bar, hint bar, and border-based selection.

### Performance Daemon

- Unix domain socket IPC with the shell.
- Per-game CPU governor profile support.
- Live telemetry foundation for CPU usage, temperature, and RAM.
- Automatic profile restore on game exit.

### System

- Boots straight into the shell.
- No display manager or desktop environment required for the target user flow.
- Autologin through getty override.
- Tested on Arch Linux inside VirtualBox.

---

## Planned Architecture Improvements

### Game Session Layer

The current launcher works for basic executables, but RainDropOS needs a stronger game session model for Steam, Proton, launchers, wrappers, and child processes.

Planned improvements:

- Replace direct process-only tracking with a `GameSessionManager`.
- Launch games inside systemd transient scopes.
- Track full process trees through cgroups.
- Detect session start, crash, and exit more reliably.
- Apply and restore performance profiles around the full game session, not just the first process.

Target flow:

```txt
RainDrop Shell
    ↓
GameSessionManager
    ↓
systemd transient scope / cgroup
    ↓
Gamescope session
    ↓
Steam, Proton, or native executable
```

### Gamescope Integration

RainDropOS should use Gamescope as the dedicated game-session compositor instead of relying only on normal fullscreen window behavior.

Planned use cases:

- Run games inside a controlled fullscreen session.
- Improve resolution handling and scaling.
- Support a cleaner console-style display pipeline.
- Prepare for future frame pacing, refresh-rate, and overlay work.

### SDL3 Migration

SDL2 is currently used for the shell prototype. SDL3 is the intended long-term windowing and input layer.

Planned improvements:

- Migrate shell windowing and input from SDL2 to SDL3.
- Update controller handling to SDL3 APIs.
- Replace SDL2_ttf with SDL3_ttf or a direct FreeType-based text renderer.
- Keep the shell rendering path simple until a dedicated UI renderer is needed.

### Audio Stack

RainDropOS needs a first-class Linux audio stack for gaming, Bluetooth devices, volume control, and future capture features.

Planned stack:

- PipeWire for audio routing and low-latency audio.
- WirePlumber for session and device policy management.
- Shell-level volume and output-device controls.
- Future per-game audio routing support.

### System Updates and Rollback

A console-style OS should survive updates without leaving the user with a broken system and a fresh reason to hate computers.

Planned update strategy:

- Short term: Arch Linux package updates with Btrfs snapshots.
- Medium term: Snapper or equivalent snapshot management.
- Long term: atomic image-style updates with rollback support.

### Application Distribution

Steam remains the primary game platform target, but RainDropOS should support optional non-Steam applications without polluting the base OS.

Planned support:

- Flatpak for optional user applications.
- Heroic Games Launcher, Discord, browsers, emulators, and utilities through sandboxed app installs.
- Keep the base OS lean and controlled.

---

## Getting Started

### Prerequisites

Current development requirements:

- Linux or WSL2.
- `clang++`.
- `cmake`.
- `ninja`.
- `libsdl2-dev`.
- `libsdl2-ttf-dev`.

Future requirements will include SDL3, Gamescope, PipeWire, WirePlumber, and systemd integration packages as those features are implemented.

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

# Daemon, optional during current development
./build/raindrop-daemon

# Tests
./build/vdf-test
./build/steam-test
```

---

## Configuration

### Game Library

Edit `library.json` in the project root to add games manually. The shell reads this file on every launch, so manual entries do not require a recompile.

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

The daemon applies the profile on game launch and restores defaults on game exit. If no profile exists for a game, system defaults are used.

Future profiles may include GPU mode, frame-rate targets, Gamescope options, audio behavior, and telemetry preferences.

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

### Current Stack

| Component | Technology |
|---|---|
| Shell | C++20 |
| Windowing / input | SDL2 |
| Font rendering | SDL2_ttf |
| JSON | nlohmann/json |
| Steam parsing | VDF parser |
| IPC | Unix domain sockets |
| Game launching | `fork`, `execvp`, `waitpid`, Steam URI launch |
| Build system | CMake + Ninja |
| Compiler | Clang 18 |
| Base OS | Arch Linux |

### Target Stack

| Component | Technology |
|---|---|
| Shell | C++20 / C++23 |
| Windowing / input | SDL3 |
| Font rendering | SDL3_ttf or FreeType |
| Game-session compositor | Gamescope |
| Game session tracking | systemd transient scopes + cgroups |
| Performance daemon IPC | Unix domain sockets with typed protocol messages |
| Optional system IPC | D-Bus / sd-bus for selected system services |
| Audio | PipeWire + WirePlumber |
| Config | nlohmann/json + schema validation |
| Application distribution | pacman for base packages, Flatpak for optional apps |
| Installer | Calamares |
| Updates | Btrfs snapshots short term, atomic rollback long term |
| Build system | CMake + Ninja |
| Compilers | Clang, GCC CI target |
| Base OS | Arch Linux development base |

---

## Roadmap

| Phase | Description | Status |
|---|---|---|
| 0 — Infrastructure | Build pipeline, toolchain, repository structure | ✅ Done |
| 1 — Base System | Arch install, autologin, boot directly to shell | ✅ Done |
| 2 — Shell | Render loop, input abstraction, library views, settings | ✅ Done |
| 3 — Steam Integration | VDF parsing, Steam library scanning, Steam launch support | ✅ Done |
| 4 — Performance Daemon | IPC, profiles, telemetry, automatic restore | 🔄 In progress |
| 5 — Game Session Manager | systemd scopes, cgroup tracking, robust launch lifecycle | ⬜ Planned |
| 6 — Gamescope Integration | Dedicated fullscreen game-session compositor | ⬜ Planned |
| 7 — SDL3 Migration | Modernized shell windowing, input, and controller handling | ⬜ Planned |
| 8 — Audio Integration | PipeWire, WirePlumber, volume/device controls | ⬜ Planned |
| 9 — Installer and Packaging | Calamares installer, package repo, install image | ⬜ Planned |
| 10 — Updates and Rollback | Btrfs snapshots, rollback-safe update flow | ⬜ Planned |
| 11 — Polish | GPU support, OSK, animations, hardware testing | ⬜ Planned |

---

## Development Priorities

Immediate priorities:

1. Finish the performance daemon IPC and profile lifecycle.
2. Introduce a `GameSessionManager` abstraction.
3. Move launched games into systemd scopes for reliable process tracking.
4. Add Gamescope launch support as an optional path.
5. Begin SDL3 migration once the shell architecture is stable.

Near-term polish:

- Better controller navigation behavior.
- More robust Steam and Proton launch handling.
- On-screen keyboard support.
- Game artwork and metadata support.
- Settings screen for performance profiles.
- Hardware testing outside VirtualBox.

---

## Contributing

RainDropOS is in active early development. Contributions, issues, and feedback are welcome.

Before submitting a large pull request, open an issue describing the change, the motivation, and the parts of the system it affects. This is especially important for platform, launcher, compositor, daemon, and installer changes.

---

## License

Released under the [Apache License 2.0](LICENSE).