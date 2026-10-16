# Jail-ed native editor

This is a source port of **Jail-ed 1.1b**, the editor bundled with **Cell Block A 1.3b**. The symbolized 1.0 executable supplied names; the shipped 1.3b editor supplied the recovered behavior and current asset indices. No emulation or DOS executable runs in the native applications.

Build both applications with `tools/build.sh` on Mac/Linux or `python tools/build_windows.py` on Windows. Launch `build/Jailed`, `Jail-ed.app`, or `Jailed.exe`. An optional map filename opens that map at startup. Every platform has a standalone editor download. Windows and Linux game downloads also include the editor.

## Install and launch

| Platform | Standalone editor download | Launch |
| --- | --- | --- |
| macOS | `Jailed-macOS-arm64.zip` or `Jailed-macOS-x86_64.zip` | Open `Jail-ed.app` |
| Windows 10/11 x64 | `Jailed-Windows-x64.zip` | Run `Jailed.exe` in the extracted folder |
| Linux | `Jailed-Linux-<architecture>.tar.gz` | Run `./Jailed` in the extracted folder |

Keep each extracted package together. Windows includes SDL2; Linux uses the system SDL2 runtime and Zenity for save/open dialogs (`sudo apt install libsdl2-2.0-0 zenity` on Ubuntu/Debian). The editor runs independently of the game. Install Cell Block A when you want to play your maps.

## Original controls

| Key | Tool/action |
| --- | --- |
| Q | Ladder |
| W | Left platform end |
| E | Middle platform |
| R | Right platform end |
| T | Start position |
| S | Save As |
| L | Load |
| N | New map |
| Escape | Quit confirmation |

Left-click places the selected piece. Drag to paint. Right-click removes the selected class of piece: ladders, platform tiles, or start points. Platform pieces use the original four random visual variants. Clicking an existing start point with T flips its facing direction. There are at most four start points; saving requires at least two. New maps retain the original two default starts. The background, 640×480 canvas, 20×13 editable area, cursors, artwork and start markers come from the original editor.

Save writes a compatible `.cel` Allegro packfile and a 200×150 `.pcx` preview. The old manual calls this a `.pic`; the actual 1.3b editor and game use `.pcx`. Native saves use valid literal-only LZSS streams, so compressed file sizes can differ while decoded bytes remain identical. Save/open dialogs use macOS or Windows dialogs. Linux optionally uses Zenity. Unsaved-change confirmations, window resizing and invalid-file rejection are deliberate native safety adaptations.

## Playing saved maps

The editor defaults to the same writable map folder the game scans:

- macOS: `~/Library/Application Support/KevlarX/CellBlockA/maps`
- Windows: `%APPDATA%\KevlarX\CellBlockA\maps`
- Linux: `~/.local/share/KevlarX/CellBlockA/maps`

`CELL_SAVE_DIR` overrides the shared parent folder for both programs. The game's map menu merges these maps with bundled maps; a user map with the same filename takes precedence. Restart the map menu after saving. The original menu supports filenames up to 70 UTF-8 bytes, including the extension; longer filenames can be edited but are omitted from the game list. Save maps in the default folder to appear in the game; files saved elsewhere can be copied there with their previews. Load can browse the original maps in the package's resource folder.

Malformed/truncated maps, out-of-range starts, and maps with fewer than two starts are rejected without replacing the current document. Invalid maps are omitted from the game's menu. Original resources are never modified by normal editor saves.

## Verification

`tools/test.sh` tests all original map round trips, all editor tools and random variants, starting points, native edge coordinates, PCX previews, failed saves, map discovery, and 1,000 game simulation ticks each for a new and an edited map, under ASan/UBSan. Windows runs the same C++ tests natively through `tools/build_windows.py --test`.

`python tools/test_jailed.py --editor <executable> --game <executable>` runs both packaged executables: create, save, load, edit, save, independently decode output, and select the edited map in a game replay. It also checks relocation to paths containing spaces and Unicode. `Jailed --script <file>` is a deterministic integration driver using the same handlers as the UI (commands: `new`, `tool q`, `click x y button`, `load path`, `save path`, `capture path`). Buttons are 1=left and 2=right.

The recovered evidence is in `analysis/jailed-1.3b/`; native generated routines are in `src/jailed_recovered.c`; the event loop/dialog integration is in `src/jailed.cpp`; shared serialization is in `src/map_io.cpp`. See [RECOVERY.md](RECOVERY.md) and [FIDELITY.md](FIDELITY.md) for provenance and limitations.
