#!/bin/sh
# make install: checks for LLVM (with clang) and Lua, offers to install the
# missing ones with the system's package manager, then builds bin/ura.
# make check (install.sh check): only reports what is found or missing.
set -e
cd "$(dirname "$0")/.."
MODE=${1:-install}
MISSING=0

ask() {
    printf '%s [y/n] ' "$1"
    read -r answer
    case "$answer" in
        y | Y | yes) return 0 ;;
        *) return 1 ;;
    esac
}

package_manager() {
    for manager in apt-get dnf pacman zypper brew; do
        if command -v "$manager" >/dev/null 2>&1; then
            echo "$manager"
            return
        fi
    done
}

install_packages() {
    case "$MANAGER" in
        apt-get) sudo apt-get install -y "$@" ;;
        dnf) sudo dnf install -y "$@" ;;
        pacman) sudo pacman -S --needed --noconfirm "$@" ;;
        zypper) sudo zypper install -y "$@" ;;
        brew) brew install "$@" ;;
    esac
}

has_llvm() {
    command -v clang >/dev/null 2>&1 || return 1
    command -v llvm-config >/dev/null 2>&1 && return 0
    for config in /usr/bin/llvm-config-* /usr/lib/llvm-*/bin/llvm-config \
        /usr/local/opt/llvm*/bin/llvm-config /opt/homebrew/opt/llvm*/bin/llvm-config \
        "$HOME"/.local/opt/llvm*/bin/llvm-config; do
        [ -x "$config" ] && return 0
    done
    return 1
}

llvm_versions() {
    for config in $(command -v llvm-config 2>/dev/null) /usr/bin/llvm-config-* \
        /usr/lib/llvm-*/bin/llvm-config /usr/local/opt/llvm*/bin/llvm-config \
        /opt/homebrew/opt/llvm*/bin/llvm-config "$HOME"/.local/opt/llvm*/bin/llvm-config; do
        [ -x "$config" ] && "$config" --version
    done | sort -u | tr '\n' ' '
}

has_lua() {
    command -v lua >/dev/null 2>&1
}

# require <name> <check> <apt-get> <dnf> <pacman> <zypper> <brew>
require() {
    name=$1
    check=$2
    if "$check"; then
        echo "$name: found"
        return
    fi
    if [ "$MODE" = check ]; then
        echo "$name: missing (make install offers to install it)"
        MISSING=1
        return
    fi
    if [ -z "$MANAGER" ]; then
        echo "$name is required and no package manager was found: install it, then run make install again"
        exit 1
    fi
    case "$MANAGER" in
        apt-get) packages=$3 ;;
        dnf) packages=$4 ;;
        pacman) packages=$5 ;;
        zypper) packages=$6 ;;
        brew) packages=$7 ;;
    esac
    if ! ask "$name is required but not installed. Install it with $MANAGER ($packages)?"; then
        echo "$name is required: install it, then run make install again"
        exit 1
    fi
    # shellcheck disable=SC2086
    install_packages $packages
}

MANAGER=$(package_manager)
echo "ura builds with LLVM 12 to 23; the recorded tests need LLVM 12 (make dev LLVM=12, or set-llvm 12 inside make dev)"
require "LLVM (with clang)" has_llvm "llvm-dev clang" "llvm-devel clang" "llvm clang" "llvm-devel clang" "llvm"
echo "  LLVM versions found: $(llvm_versions)"
require "Lua" has_lua "lua5.4" "lua" "lua" "lua54" "lua"

if [ "$MODE" = check ]; then
    if [ -x bin/ura ]; then echo "bin/ura: built"; else echo "bin/ura: not built (make install)"; fi
    exit "$MISSING"
fi

lua config/main.lua build
echo "ura is installed: run make dev to open the development shell"
