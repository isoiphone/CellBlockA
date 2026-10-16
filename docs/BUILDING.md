# Building and testing

Both game and editor are built together. See [JAILED.md](JAILED.md) for editor controls and shared map storage.

The normal build inputs are the 1.3b distribution in `original-1.3b/`, saved 1.3b typed recovery, native C/C++ sources, and vendored JGMOD source. No decompiler, JDK, DOS tools, or original development environment is required. Work from a complete clone or extracted source archive, in the repository root. Python 3.9+ is required; Git LFS and submodules are not used.

## macOS

Install Apple's command-line tools and Homebrew dependencies:

```sh
xcode-select --install
brew install python sdl2-compat sdl3
./tools/build.sh
./build/CellBlockA
./build/Jailed
```

Build and package for the current Mac's architecture:

```sh
./tools/package.sh
open "dist/Cell Block A.app"
```

Outputs: `dist/Cell Block A.app`, `dist/Jail-ed.app`, `dist/CellBlockA-macOS-<architecture>.zip`, and `dist/Jailed-macOS-<architecture>.zip`. Intel and Apple Silicon builds are separate, not a universal binary. The scripts discover library paths rather than assuming `/opt/homebrew` or `/usr/local`. Packaging bundles non-system dylibs, adds notices, derives the minimum macOS version from the binaries, and ad-hoc signs the app.

A Homebrew build may require a newer macOS release than the game itself needs. Building on an older supported Mac with compatible SDL libraries is the route to an older deployment target; setting a lower plist value does not make newer libraries compatible. Notarization and Developer ID signing are not performed.

## Windows

See [WINDOWS.md](WINDOWS.md). In PowerShell:

```powershell
py -3 tools/build_windows.py --test
```

The script downloads pinned, hash-verified Zig 0.14.1 and SDL 2.32.10 archives from official sources. It targets x64 Windows 10/11 with Clang/MinGW, and creates `CellBlockA-Windows-x64.zip` and `Jailed-Windows-x64.zip`, each with a complete portable folder. The game folder contains CellBlockA.exe and Jailed.exe; the standalone editor folder contains Jailed.exe and its dependencies; a console test executable is built separately. MSVC is not a supported compiler for this recovered-source build.

The same Python script without `--test` cross-compiles Windows binaries from Apple Silicon or Intel macOS. Native Windows tests cannot execute on Mac; no emulator is used.

## Linux

On Ubuntu 24.04 / Debian-like systems:

```sh
sudo apt update
sudo apt install clang python3 libsdl2-dev zenity
./tools/build.sh
./build/CellBlockA
./build/Jailed
```

Other distributions need equivalent development packages providing Clang/Clang++, Python 3.9+, `sdl2-config`, SDL2 headers and its shared library. The code requires C11/C++17. The build uses the host CPU architecture; x64 and arm64 are intended targets, but only Ubuntu x64 has a configured CI job.

```sh
./tools/package.sh
```

This creates `dist/CellBlockA-Linux-<architecture>.tar.gz` containing both CellBlockA and Jailed, plus `dist/Jailed-Linux-<architecture>.tar.gz` containing the standalone editor and its resources. The package includes the game resources and uses the system SDL2/C++/libc runtime. Install Zenity for Linux editor file dialogs. It is not an AppImage or a universal Linux binary. CI builds on Ubuntu 24.04; build on the target distribution for best compatibility.

## Tests

Mac/Linux:

```sh
./tools/test.sh
```

This rebuilds with AddressSanitizer and UndefinedBehaviorSanitizer, then runs the original-layout, RNG, math, collision-boundary, redraw-boundary, damage, particle, all-map simulation, and all-track music tests with SDL dummy drivers. Leak detection is disabled for platform/library lifetime allocations; memory bounds and undefined-behavior checks remain enabled. It replaces `build/CellBlockA`; `tools/package.sh` always rebuilds without sanitizer/test flags.

Windows:

```powershell
py -3 tools/build_windows.py --test
# Or, after building:
py -3 tools/test_windows.py
```

Windows tests are native but do not use sanitizers. The package smoke test also checks resource discovery after relocation to a path containing spaces and Unicode. Headless tests do not verify real audio output, GPU rendering, keyboard rollover, or original-DOS equivalence.

Shared repository validation and source export:

```sh
python3 tools/check_repo.py
python3 tools/export_source.py
python3 tools/test_source.py          # Verify the archive in an isolated directory
```

## Build overrides

Mac/Linux build scripts accept `CC`, `CXX`, `PYTHON`, and `SDL2_CONFIG`, each naming one executable. Defaults are `clang`, `clang++`, `python3`, and `sdl2-config`. `SDL3_PREFIX` can point to a nonstandard SDL3 prefix for Mac builds. These variables are not shell command strings. The supported/tested compiler is Clang; GCC and other compilers have not been validated.

Developer-only runtime settings:

| Variable | Purpose |
| --- | --- |
| `CELL_RESOURCE_DIR` | Override the directory containing extracted `assets`, `maps`, and `original` |
| `CELL_SAVE_DIR` | Override user settings/screenshot directory |
| `CELL_VERIFY=1` | Run regression tests in a test-enabled executable |
| `CELL_INPUT_SCRIPT` | Timed replay file: milliseconds, original scan code, pressed/released |
| `CELL_TEST_SECONDS` | Exit a scripted launch after this many seconds |
| `CELL_TEST_CAPTURE` | Save the indexed framebuffer as a BMP at scripted exit |

Do not build or package concurrently from the same checkout: extraction and source generation share output paths. Use separate checkouts for concurrent platform builds. Build scripts and source export operate only on native-game paths and do not use the website directory.

Replay timestamps must be nondecreasing, scan codes must be 0–127, and key states must be 0 or 1. Invalid or incomplete replay records are rejected before gameplay. Package smoke tests also exercise these rejection cases.

## Generated files and cleanup

`build/` and `dist/` contain disposable binaries, packages, logs, and downloaded Windows dependencies. Builds also recreate ignored COFF images, extracted assets/maps, and the resource symlink under `analysis/`. Downloaded Ghidra/JDK folders under `vendor/` are needed only for optional recovery research. None of these are source inputs.

Keep the tracked original distributions, SHA-256 manifests, typed recovery evidence, generated source, and vendored JGMOD source/notices. The saved evidence makes normal builds independent of the recovery tools. The historical ZIPs and websites are intentional preservation artifacts.
