# Recovery and source layout

The native build targets **Cell Block A 1.3b**, the version stated in the executable and manual inside `cellin13.exe`. Its manual is dated February 4, 2000. The Windows self-extracting installer contains a ZIP of DOS programs and data; extraction reads ZIP bytes without executing the installer. Both CELL.EXE and JAILED.EXE are DJGPP COFF programs.

The earlier reconstruction used 1.0 by mistake. Its executable retains COFF symbols, while 1.3b is stripped. Normalized x86 instruction alignment transferred candidate function/global names from 1.0; instruction accesses, calls and typed Ghidra decompilation recovered all 64 game functions from 1.3b. Short, similar CRT/keyboard functions and sprite-flip wrappers required manual disambiguation. Candidate names outside the recovered game should not be treated as independently verified symbols.

## Files needed for normal builds

| Path | Role |
| --- | --- |
| `original-1.3b/` | Current distribution: all 39 installer files, bytes unchanged, filenames uppercased |
| `analysis/original-1.3b-sha256.json` | Expected hashes of current inputs |
| `analysis/types-1.3b.h` | Recovered structures and function declarations |
| `analysis/typed-1.3b/` | All 64 typed game functions, with original 1.3b addresses |
| `analysis/recovery-1.3b.json` | Installer hash, version, addresses, constants and asset counts |
| `analysis/symbols-1.3b.tsv` | Candidate labels transferred from 1.0, with verified CRT/keyboard/mirroring corrections |
| `original/`, `analysis/types.h`, `analysis/typed/` | Earlier 1.0 distribution and research evidence; not current game logic |
| `analysis/original-sha256.json` | Expected hashes of the 1.0 reference files |
| `tools/extract_original.py` | Validate both distributions; extract current COFF, Allegro archives and maps |
| `tools/generate_native.py` | Reproducible source adaptations, exact constants and lookup tables |
| `src/recovered.c` | Generated 1.3b native C game logic |
| `src/platform.cpp` | Native graphics, input, sound, timing, filesystem and entry point |
| `src/state.c` | Game globals and original initial state |
| `src/allegro.h` | Small host API shim used by vendored JGMOD |
| `vendor/JGMOD-main/src/` | JGMOD 0.99 tracker playback source |
| `tests/core.cpp` | Game logic, asset and memory-boundary regression tests |

Normal builds regenerate the current COFF images, extracted assets/maps and generated headers/C. They require no Ghidra, Java, Capstone, DOS runtime or emulator. The saved typed evidence is adapted for native pointer widths and host APIs, with the documented safety corrections. Decompiled evidence stays separate from those deliberate port changes.

The embedded version is checked directly before generating source. Repository tests also compare the complete horizontal-mirroring wrapper with the symbolized 1.0 executable; a similar-looking vertical-flip wrapper caused a rendering regression in the initial 1.3b recovery. Constants are read using COFF section addresses and file offsets; 1.0 and 1.3b do not have the same section file offsets. The sine/tangent tables and random-number algorithm were checked against both releases and are unchanged.

## Repeating the reverse engineering

This optional research uses Ghidra 12.1.4 and Temurin JDK 21. Run `python3 tools/extract_original.py`, then import the unmodified `analysis/CELL.coff` into a Ghidra project. Run `tools/Recover13.java` with three script arguments: `analysis/types-1.3b.h`, `analysis/symbols-1.3b.tsv`, and a separate output directory. Use absolute paths for those arguments when invoking Ghidra from elsewhere. The script applies candidate labels, manually corrected CRT, keyboard and horizontal-mirroring targets, recovered structures and prototypes, then exports the 64 game functions for comparison with `analysis/typed-1.3b/`.

The older `tools/Recover.java` and `tools/TypeRecovery.java` belong to the 1.0 investigation. Do not apply the 1.0 types/addresses to the current image. `CELL-normalized.coff` exists solely for LLVM disassembly; Ghidra must use the unmodified COFF image. Local project databases, downloaded tools and exploratory output are excluded from the public source set.

In the full repository, `python3 tools/extract_release13.py` verifies the archived installer hash and all extracted files. Its optional `--extract` flag restores `original-1.3b/` without executing historical code. The standalone source export already contains the 39 verified files and does not require the website or installer.

See [FIDELITY.md](FIDELITY.md) for intentional corrections and limits on equivalence to original DOS execution.

## Jail-ed recovery

The editor embedded in the 1.3b installer identifies itself as **1.1b** at COFF address `0x2368`. `analysis/jailed-recovery.json` records both executable hashes, all 13 recovered application function addresses and evidence hashes, the five compiled core routines, and native adaptations. `analysis/jailed-1.0/` preserves the symbolized reference decompilation; `analysis/jailed-1.3b/` contains typed current evidence. Candidate transferred labels are in `analysis/jailed-symbols-1.3b.tsv`; they are research candidates outside verified editor targets.

For a typed export, import the unmodified `analysis/JAILED.coff` into Ghidra and run `tools/RecoverJailed.java` with three absolute-path arguments: `analysis/jailed-symbols-1.3b.tsv`, a separate output directory, and `analysis/jailed-types-1.3b.h`. It assigns labels/types and exports editor functions in `0x1574..0x2ffc`. To export the earlier reference, pass its COFF symbol TSV and output directory, omitting the third argument (the typed addresses apply only to 1.3b). The old horizontal-flip wrapper at `0xa6b0` is identical to current `0xa6bc`; the nearby vertical-flip candidate was corrected by this byte comparison.

`tools/generate_jailed.py` adapts reset, click, tile drawing, map drawing and preview routines without a decompiler at build time. It widens pointer casts, namespaces the editor functions and bounds mouse coordinates. `src/jailed.cpp` replaces DOS initialization, the interrupt-driven input loop, and Allegro GUI calls with native host services. `src/map_io.cpp` implements the recovered 260 tile bytes plus four three-byte starts and original Allegro packfile/PCX formats, with validation before mutating state. The native game uses this same decoder to play editor saves without asset re-extraction.
