#!/usr/bin/env bash
set -e

# Ensure required build tools are available
REQUIRED_TOOLS=(g++ nasm grub-mkrescue xorriso)
for tool in "${REQUIRED_TOOLS[@]}"; do
    if ! command -v "$tool" > /dev/null 2>&1; then
        echo "Error: $tool is required but not installed." >&2
        exit 1
    fi
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

pushd "$ROOT_DIR" > /dev/null
make iso
popd > /dev/null