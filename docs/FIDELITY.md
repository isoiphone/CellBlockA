# Fidelity record

## Basis of the reconstruction

The native reconstruction targets **1.3b**, identified by both the executable and manual inside `cellin13.exe` (February 4, 2000). The installer is a Windows self-extracting ZIP containing DOS executables; extraction never runs it. Its 39 files are preserved byte-for-byte in `original-1.3b/`, with filenames uppercased for DOS-compatible lookup.

The earlier reconstruction incorrectly used 1.0. Its symbolized executable remains in `original/` as a research reference. The 1.3b executable is stripped: normalized x86 instruction alignment transferred candidate names from 1.0, then Ghidra 12.1.4 recovered all 64 game functions. Types and call sites were checked, including manual correction of ambiguous short CRT/keyboard and sprite-mirroring functions. Current evidence is in `analysis/typed-1.3b`; earlier evidence remains in `analysis/typed`. See [RECOVERY.md](RECOVERY.md) for reproducible inputs. No gameplay was implemented from a visual description or redesigned.

`src/recovered.c` retains the recovered control flow for menus, map selection, player movement, jumping/rolling, attacks, damage, deaths, match handling, particles, item spawning, and music selection. Address comments connect the functions to the executable. Compiler options preserve signed `char` and wrapping integer arithmetic.

The Allegro LZSS archives were unpacked into their original object payloads: 469 GAME objects and 25 MENU objects. The native loader reads their original indexed pixels, fonts, samples, and palettes. All 12 supplied maps, their PCX previews, and eight MOD files are retained. No replacement art or sound assets were generated. The 1.3b logic supplies its intro and help screens, expanded player voices, death handling and randomized arena backdrop. The DEATH map is included. Of the 130 game sound samples, 119 are 16-bit and 11 are 8-bit; their raw PCM payloads are retained.

The original 64-bit random-number generator, particle initialization words, fixed-point conversion rules, and sine/cosine/tangent tables were recovered. Palette fades follow the original interpolation formula.

## Corrected sprite recovery error

The first 1.3b build wrongly named the helper at `0x13c48` as `_draw_sprite_v_flip`. This flipped characters and platform end pieces upside down. The complete 29-byte helper is identical to 1.0's `_draw_sprite_h_flip` at `0x13474`: its indirect call uses bitmap-vtable offset `0x30`; vertical flip uses `0x2c`. The recovered label, metadata and four affected decompiled functions have been corrected and regenerated. This restores original behavior; it is not an intentional gameplay or graphics change.

All 299 character/marker images and all nine platform images were compared byte-for-byte with 1.0 at their relocated indices. The regression first failed on tile kind 14 in the broken build. Corrected rendering is checked against independent pixel composition from the original assets, not merely whether a drawing helper runs without crashing. `build/test-save/rendering-regression.pcx` records a deterministic native-rendered contact sheet for visual inspection. Earlier smoke-test success did not establish visual correctness.

## Approved memory-safety changes

The author explicitly approved fixing both map-boundary bugs rather than preserving their accidental behavior. Collision queries outside the 640×416 map return empty space, checked before converting pixels to tile indices. They cannot read the next column or player data. Existing player boundary handling and falling-death logic remain unchanged.

Redraw marking clips its inclusive pixel rectangle to the map before iterating over tiles. Fully outside or inverted rectangles do nothing. It cannot write before or after the map. This also marks every overlapping tile, including the right/bottom rectangle edge. The map and players are now separate arrays; the former compatibility memory block and raw-byte accessors have been removed. Original decompiler evidence remains unchanged in `analysis/typed-1.3b`.

## Native adaptations and unresolved equivalence

| Area | Current implementation / limit |
| --- | --- |
| Display | 640×480 indexed drawing and original palette values, converted to RGB for SDL. Integer scaling uses nearest pixels. Physical CRT appearance and refresh timing are not reproduced. |
| Timers | Original callback intervals are scheduled on the native event loop in deadline order. DOS interrupt preemption is not reproduced. Retrace waits currently use a 16 ms delay. Wall-clock/input scheduling and stalls can differ from the DOS machine. |
| Keyboard | Original gameplay scan codes are mapped from physical SDL keys. Native event delivery, repeat, and rollover depend on the host keyboard/OS. No replacement laptop music shortcuts were invented. |
| Audio | Original sample bytes and MOD files are retained. The MOD engine is JGMOD 0.99 (2002), newer than the original game, compiled natively. The native 44.1 kHz floating-point stereo mixer is a replacement for Allegro/Sound Blaster output. Exact tracker behavior, mixing, levels, resampling and timing are not verified equivalent. |
| Random startup | The original random sequence is preserved for a given seed. Startup uses a native clock seed, so a fresh launch need not choose the same map positions/music as a historical DOS launch. |
| Floating point | Original constants are extracted bit-for-bit. Native calculations do not reproduce every detail of the x87 extended-precision execution environment. |
| Filesystem | Native directory enumeration replaces DOS enumeration, so map/music ordering is not certified against the original filesystem. Saves use the platform's user data directory. Packed maps are decoded at runtime. Bundled and user maps are merged in filename order; user maps take precedence. Invalid maps are excluded. |
| Memory layout | Gameplay scalar widths are preserved; native pointers and linked lists use host widths. Map access is explicitly bounded as an approved safety correction. Original map/player aliasing and redraw writes outside the map are no longer reproduced. |
| Match-time text | The decompiled routine exposes an uninitialized output pointer; the native adaptation supplies a stable buffer while retaining the original format and calculations. |
| Editor | Jail-ed 1.1b from the 1.3b distribution is recovered using 1.0 symbols. Its original editing/rendering routines run natively. OS file dialogs, unsaved-change confirmation, mouse bounds checks, validated map I/O and safe file replacement are host adaptations. Literal-only LZSS saves preserve original decoded bytes, not compression choices. |

These are limits on an identical-copy claim. The current deliverable is a playable native reconstruction with recovered game logic, not a certified bit-exact port.

## Verification performed

- Jail-ed preserves all 13 recovered application functions as evidence; five editing/rendering routines compile directly after documented native adaptations. Both executable hashes and the embedded editor version are checked. The horizontal-flip wrapper is verified byte-for-byte against the symbolized 1.0 binary.
- Editor tests cover all tools/variants, default starts, direction toggling, erasing, outside-window coordinates, all 12 original map round trips, preview pixels and decoding, invalid input and failed saves. New and edited maps each run 1,000 gameplay simulation ticks under sanitizers.
- Both packaged macOS programs passed a relocated Unicode-path integration test: create/save/load/edit/save in Jailed, independently unpack the saved files, then play the edited map through the normal game menu (274 active game ticks in the recorded replay). A native macOS window session also verified mouse placement, Save As, New and Load. These checks do not certify original-DOS frame equivalence or Windows runtime behavior.


- Both original SHA-256 manifests are checked by extraction/build; all 41 earlier files and 39 current files remain unchanged. Repository checks and source generation verify the embedded 1.3b version. The installer is separately checked against its recorded hash and extracted files.
- Boundary regression tests cover all 256 tile values, every map tile's corners, outside coordinates including negative and integer extremes, and partially/fully outside redraw rectangles. They verify exact dirty-tile coverage and unchanged tile kinds, player data and file-list state.
- Native clock conversion is tested with long-uptime counter values to avoid overflow. This correction changes the host time conversion, not the game's scheduled tick intervals.
- Developer replay input is validated before indexing keyboard state. Invalid replay records are rejected; ordinary controls and valid replay behavior are unchanged.
- Native arm64 compilation and standalone app packaging succeed. Runtime dependencies resolve to bundled SDL libraries and macOS system libraries. The app passes `codesign --verify --deep --strict`.
- AddressSanitizer and UndefinedBehaviorSanitizer pass the regression suite: all 12 maps, 500 player updates and 1,000 particle updates per map, movement/jumps/rolls, four weapon selections, rendering, damage/death, timed item creation, and music list wraparound.
- Tests load all 469 GAME and 25 MENU objects and check every sound sample format. Pixel regressions check all 256 tile kinds, all player animation frames for both players/directions, and the six directional particle sprite frames in both directions.
- A scripted native 1.3b launch traverses both intro screens, the main menu and map selection, then enters gameplay; framebuffer capture and player/tick state are saved under `build`. The captured framebuffer was visually inspected.
- The earlier 1.0 reconstruction received physical window/input checks on Mac. Those checks do not certify the new 1.3b build: its current automated checks use headless SDL; fresh interactive audio/input testing remains pending.
- Commit `99c0fb3` passed [native Windows CI](https://github.com/isoiphone/CellBlockA/actions/runs/39007043354): regression assertions, packaged gameplay, malformed replay rejection, and standalone editor-to-game integration. Unicode application, save, input and capture paths are covered. Windows packages include SDL 2.32.10; no DOS executable is included.
- The same commit passed [Ubuntu x64 and Intel/Apple Silicon Mac CI](https://github.com/isoiphone/CellBlockA/actions/runs/39007043357), including sanitizer regressions and packaged game/editor integration. These runs use headless SDL; interactive graphics, audio, keyboard and native-dialog testing on Windows, Linux and Intel Mac remains pending. Linux arm64 is not covered by CI.
- One UI-automation session reported that the app quit. Repeating the control checks succeeded; no corresponding game crash report was found. A separate shell GUI launch aborted in macOS application registration under the shell sandbox, before gameplay. These observations are not treated as proof of long-run stability.

There has been no frame-by-frame, state-by-state, or audio comparison against execution on original hardware. The original executable was not run in an emulator. Such a reference comparison, plus recovery of the original sound engine/mixer and timer behavior, remains necessary before claiming identical behavior.

## Tool and library provenance

Ghidra 12.1.4 was obtained from the official National Security Agency Ghidra release and run locally with Temurin JDK 21. The arm64 decompiler was compiled from its bundled source. The scripts and typed evidence are included in the public source set; the local Ghidra database and downloaded tool distributions are excluded and unnecessary for normal builds.

The JGMOD 0.99 source is retained in `vendor/JGMOD-main`, obtained from the ArtificialRaccoon/JGMOD repository. Its original `readme.txt` identifies the version and licensing terms. SDL2-compat and SDL3 are the installed Homebrew libraries copied into the app. The native app does not need the Ghidra/JDK tooling or the original DOS executable at runtime.

The Windows build uses the official Zig 0.14.1 C/C++ toolchain and SDL 2.32.10 MinGW package, pinned by SHA-256 in `tools/build_windows.py`. Windows uses SDL2 directly; Mac currently uses SDL2-compat backed by SDL3. Display/input/audio behavior between those backends remains subject to platform testing.
