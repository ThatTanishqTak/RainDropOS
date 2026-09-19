# RainDropOS

An experimental **Arch Linux-based gaming environment** designed around a console-style game library.

RainDropOS combines a C++20/SDL2 shell, Steam library discovery, native game launching and a background performance daemon. The repository also contains an ArchISO profile and session configuration for booting into the shell.

**Status:** Development prototype. The shell can be built independently; installer, compositor and system-update workflows remain unfinished.

## Current Features

### Gaming Shell

* SDL2 rendering and SDL2_ttf text.
* Keyboard and controller navigation.
* Library, game-detail and settings screens.
* Steam library discovery through VDF parsing.
* Manual game entries supplied through JSON.
* Native executable launching.
* Steam launching through `steam://rungameid/<appid>`.
* Configurable window and fullscreen behaviour.

### Performance Daemon

* Unix-domain socket communication with the shell.
* Game-start and game-stop messages.
* CPU governor profile handling.
* CPU, memory and temperature telemetry foundations.

### System Integration

* ArchISO configuration.
* Session startup and autologin configuration.
* systemd service definitions.

## Requirements

For the development build:

* Linux.
* A C++20 compiler.
* CMake 3.20 or newer.
* Ninja.
* SDL2 development files.
* SDL2_ttf development files.

A graphical session is required to run the shell. CPU governor control additionally requires Linux cpufreq support and appropriate permissions.

## Build and Run

```bash
git clone https://github.com/ThatTanishqTak/RainDropOS.git
cd RainDropOS

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build

./build/src/shell/raindrop-shell
```

Run from the repository root so the shell can find `library.json` and the bundled font.

The daemon can be launched separately:

```bash
./build/raindrop-daemon
```

## Manual Game Library

Add native games to `library.json`:

```json
{
  "games": [
    {
      "title": "My Game",
      "executable": "/absolute/path/to/my-game"
    }
  ]
}
```

Replace the example executable with an installed game binary.

The current native launcher passes the executable path without a separate argument list. Steam games are discovered separately and merged into the library.

## Controls

| Keyboard   | Controller | Action             |
| ---------- | ---------- | ------------------ |
| Arrow keys | D-pad      | Navigate           |
| Enter      | A          | Confirm            |
| Escape     | B          | Back               |
| Tab        | Start      | Open settings/menu |

## Performance Profiles

Profiles contain a CPU governor selection:

```json
{
  "cpu_governor": "performance"
}
```

The current implementation reads profiles from:

```text
~/.config/raindrop/profiles/<appid>.nlohmann::json
```

The `.nlohmann::json` suffix is a source-code naming bug. Conventional `.json` filenames will require correcting that path in `ProfileManager.cpp`.

On game stop, the daemon restores its configured default governor. Preserving and restoring the exact previous per-CPU settings remains future work.

## Development Checks

The build provides separate diagnostic executables:

```bash
./build/vdf-test
./build/steam-test
./build/ipc-test
./build/ipc-comms-test
./build/telemetry-test
./build/profile-test
```

Steam, telemetry and profile checks depend on the host environment.

## Current Limitations

* Launching waits for the immediate child process and can block the shell.
* Steam and launcher processes do not necessarily represent the full game session.
* Per-game profile identification and lifecycle handling need further work.
* Gamescope integration, robust session tracking, SDL3 migration and audio-device controls are planned.
* Installer tooling and rollback-safe updates are incomplete.

## Repository Layout

* `src/shell/`: Shell, navigation, rendering and game discovery.
* `src/daemons/`: IPC server, telemetry and profiles.
* `include/`: Shared protocol definitions and JSON dependency.
* `tests/`: Development checks.
* `iso/`: ArchISO and session configuration.
* `assets/`: Shell resources.

## License

Licensed under the [Apache License 2.0](LICENSE.txt).
