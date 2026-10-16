# Playing Cell Block A

Two players share a keyboard. The objective is to kill your opponent using weapons, fists, or a shove off a platform. There is no new single-player AI, networking, controller support, or key-remapping layer.

## Launch

- **Mac:** extract the matching architecture's ZIP and open `Cell Block A.app`. Its required macOS version depends on the machine/libraries used to build it. The initial locally verified build requires macOS 26. The development app is ad-hoc signed, not notarized.
- **Windows:** extract the complete ZIP and open `CellBlockA.exe`. Keep `SDL2.dll` and `resources` beside it. Target: Windows 10/11 x64.
- **Linux:** extract the archive and run `./CellBlockA` from a desktop terminal. Install the system SDL2 runtime (Ubuntu/Debian: `sudo apt install libsdl2-2.0-0`). A binary built on a newer distribution may not run on an older one; build from source on that machine if needed.

A source checkout can run `./build/CellBlockA` after `tools/build.sh` on Mac/Linux. See the [Windows instructions](WINDOWS.md).

The 1.3b intro has two screens; press a key to advance each. The original `-nointro` command-line option skips them. `-credits` prints the original real-name credits and exits.

## Controls

| | Move | Action |
| --- | --- | --- |
| Player 1 | W/A/S/D | Q |
| Player 2 | Arrow keys | Control |

Arrow keys and Enter operate menus; Escape leaves a match/menu. Numeric-keypad music keys: Num Lock random, `/` previous, `*` next, `-` pause/resume, `+` stop/start. These are physical keypad keys, not the similarly printed top-row keys. A keyboard with a numeric keypad is needed for all original music controls.

## Settings and screenshots

The original data is read-only. Settings and PCX screenshots are stored in the platform's user data directory:

| Platform | Default location |
| --- | --- |
| macOS | `~/Library/Application Support/KevlarX/CellBlockA/` |
| Windows | `%APPDATA%\KevlarX\CellBlockA\` |
| Linux | `$XDG_DATA_HOME/KevlarX/CellBlockA/`, normally `~/.local/share/KevlarX/CellBlockA/` |

Exit through the game's Quit menu to save changed options. Closing the OS window exits immediately and does not go through the options-save path.

## Troubleshooting

- **Missing resources:** keep the entire extracted package together. In a source checkout, rebuild to regenerate extracted assets; do not edit `original-1.3b/` to resolve the error.
- **No sound / sound initialization error:** confirm that the OS has a working audio output. The original `-nosound` command-line argument is retained, for example `./CellBlockA -nosound` or `.\CellBlockA.exe -nosound`.
- **Movement keys seem to conflict:** some keyboards cannot report certain simultaneous key combinations. Test with another keyboard; this project preserves the original two-player controls.
- **Large black borders or sharp pixels:** the original 640×480 indexed image uses integer scaling without smoothing. Window size and display scale determine its borders.
- **The original-input hash check fails:** restore the supplied original files. The build intentionally refuses to silently accept changed evidence.

Please report your OS version, CPU architecture, how you built/downloaded the game, map, reproduction steps, and relevant logs. Do not claim a DOS-fidelity regression solely from differences in the native audio mixer; known limits are recorded in `FIDELITY.md` in release packages and `docs/FIDELITY.md` in source checkouts.

## Creating maps

Launch Jail-ed alongside the game. Maps saved in its default user map folder appear in the game map menu when reopened. See the [editor guide](JAILED.md) for original controls and map/preview details.
