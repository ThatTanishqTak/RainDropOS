# RainDropOS

**RainDropOS** is a bespoke Linux environment crafted for gaming alone. The project draws inspiration from SteamOS while maintaining a minimal footprint.

## Overview
- **Arch Linux base**: Step 1 provides a tiny root filesystem assembled via `pacstrap`.
- **C++17 components**: Core services such as `RainDropLauncher` will be written in modern C++.
- **Consistent naming**: All binaries and scripts carry the `RainDrop` prefix for clarity.

## Getting Started
1. Build the base environment:
   ```bash
   ./scripts/RainDropBaseBuild.sh <target-directory>
   ```
   After completion, `<target-directory>` contains a minimal Arch root ready for customization.
2. Compile helper tools on your host:
   ```bash
   cmake -S . -B build
   cmake --build build
   ./build/src/RainDropBaseCheck/RainDropBaseCheck
   ```
   You should see `RainDropOS base environment ready.` printed to the console.

## Directory Layout
- `scripts/` – Automation utilities such as `RainDropBaseBuild.sh`.
- `src/` – C++ sources. Currently includes `RainDropBaseCheck` for validation.
- `docs/` – Additional documentation like `Step1_BaseDistribution.md`.

## License
RainDropOS is released under the MIT License. See [LICENSE](LICENSE) for details.
