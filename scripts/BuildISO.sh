#!/usr/bin/env bash
set -e

# Ensure required build tools are available, install if possible
REQUIRED_TOOLS=(g++ nasm grub-mkrescue xorriso)
MISSING=()
for tool in "${REQUIRED_TOOLS[@]}"; do
    if ! command -v "$tool" > /dev/null 2>&1; then
        MISSING+=("$tool")
    fi
done

if [ ${#MISSING[@]} -ne 0 ]; then
    if command -v apt-get > /dev/null 2>&1; then
        echo "Installing missing dependencies: ${MISSING[*]}" >&2
        sudo apt-get update && sudo apt-get install -y "${MISSING[@]}"
    else
        echo "Error: missing tools: ${MISSING[*]}" >&2
        exit 1
    fi
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

pushd "$ROOT_DIR" > /dev/null
make iso
popd > /dev/null