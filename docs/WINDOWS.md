# Cell Block A and Jail-ed for Windows

Extract the entire `CellBlockA-Windows-x64.zip`, then double-click `CellBlockA.exe` inside the extracted folder. Keep `SDL2.dll` and the `resources` folder beside the executable. This is a native Windows x64 C/C++ build; it does not run the original DOS executable or require an emulator.

For the standalone editor, extract `Jailed-Windows-x64.zip` and run `Jailed.exe`. It includes its own `SDL2.dll`, resources, and notices; a game installation is not required to edit maps.

Target: 64-bit Windows 10/11. The initial Windows release is cross-compiled on macOS. It has not yet been play-tested on a Windows machine; native runtime verification is provided below. The shared source has passed the Mac sanitizer suite. The app is unsigned.

Use the arrow keys and Enter in the menus. Player 1 uses WASD and Q; player 2 uses the arrow keys and Control. Original numeric-keypad music controls are retained. The original manual is `resources/original/README.TXT`.

Settings and screenshots normally go under `%APPDATA%\KevlarX\CellBlockA\`. The game's original graphics, maps, sounds, mechanics and controls are shared with the Mac port, including the two approved map-boundary memory-safety fixes. See `FIDELITY.md` for remaining fidelity limits.

## Build from source on Windows

Install Python 3.9 or later, open PowerShell in the project directory, and run:

```powershell
py -3 tools/build_windows.py --test
```

The script downloads Zig 0.14.1 and the SDL 2.32.10 MinGW development package from their official sites, checks pinned SHA-256 hashes, and caches them under `build/`. Zig supplies Clang, the Windows headers and linker; Visual Studio, MSYS2, CMake and a global Zig installation are not required. Internet access is needed on the first build. The recovered game source remains C/C++; Zig is only the compiler toolchain.

Outputs:

- `dist/CellBlockA-Windows-x64/CellBlockA.exe` and its portable resource folder.
- `dist/CellBlockA-Windows-x64.zip`, containing both programs.
- `dist/Jailed-Windows-x64/Jailed.exe` and `dist/Jailed-Windows-x64.zip`, the standalone editor package.
- `build/windows/CellBlockA-tests.exe`, a console test executable.
- `build/windows/core-test.log` and `smoke-test.log` when `--test` runs.

`--test` runs the recovered-logic regression suite, then a scripted menu-to-gameplay launch of the packaged executable from a different working directory and a path containing spaces and Unicode. It uses SDL's headless video/audio drivers. It does not replace checking real Windows graphics, audio and keyboard input.

To repeat tests without rebuilding:

```powershell
py -3 tools/test_windows.py
```

To build without running tests, omit `--test`. On an Apple Silicon or Intel Mac, the same script cross-compiles the Windows release:

```sh
python3 tools/build_windows.py
```

The Mac build and packaging commands remain `tools/build.sh` and `tools/package.sh`. No original files are modified by either build.

Compiler: <https://ziglang.org/download/>

SDL binaries: <https://www.libsdl.org/release/>

SDL and JGMOD notices are included under `Licenses` in the Windows package.

## Map editor

The portable folder also includes `Jailed.exe`. It uses the original editor tools and native Windows file dialogs. See [JAILED.md](JAILED.md) for controls and where to save maps for the game. The CI test run relocates the standalone editor and game packages separately, creates and edits a map with Jailed, then selects it in the game. Local cross-compilation is not Windows runtime verification.
