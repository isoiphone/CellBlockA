# License scope and third-party notices

## Cell Block A source

Jacob Schwartz, the original game's author, authorized the MIT release of the recovered game source and this native reconstruction in October 2026. The root `LICENSE` applies to author-owned game and editor code, native ports, recovery/build/test tools, and new documentation. It does not replace the notices or terms for the materials listed below. Preserve these notices when distributing source or binaries.

## Jacob Schwartz's assets — CC BY-SA 4.0

Jacob Schwartz created the original game/editor artwork, maps, and sound effects and licenses them under **Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)**. See `ASSET_LICENSE.md` for the scope, attribution, music exclusion, and links to the license terms. This applies to the artwork and sound effects in their original archives and in extracted/converted form; these assets are not covered by the MIT code license.

`original-1.3b/` contains the unchanged files from `cellin13.exe`, verified by `analysis/original-1.3b-sha256.json`. Native packages include its `README.TXT` unchanged. `original/` and `analysis/original-sha256.json` preserve the earlier 1.0 distribution separately. The historical freeware wording is preserved as evidence and does not override Jacob Schwartz's new MIT grant for author-owned game code or CC BY-SA 4.0 grant for his assets. Third-party material is not relicensed by either grant.

The 1.3b manual credits Thomas Craft (Protomas) and David Bronough (Necros) for vocals. Retain those credits; the author-owned asset grant does not assert ownership of third-party contributions. Confirm the scope of any third-party vocal rights before public redistribution.

## Third-party MOD music — excluded from Jacob Schwartz's licenses

The MOD music was obtained elsewhere and was **not created by Jacob Schwartz**. All MOD music files, including their embedded instrument samples, are explicitly excluded from both the MIT code license and any Creative Commons license granted by Jacob Schwartz for the game's artwork and sound effects. Inclusion in the game does not grant a new license to these tracks; their respective copyright holders retain their rights.

The original manual credits the music to various artists. The supplied distribution does not include a complete artist/license record for each track. Preserve the files and their embedded credits; neither the code license nor the asset license grants permission to separately sell or relicense this music.

| Original file | Embedded song title |
| --- | --- |
| `CARS.MOD` | Cars Kill People |
| `FALL1.MOD` | One Must Fall! 1 |
| `FALL4.MOD` | One Must Fall! 4 |
| `IGNITION.MOD` | Ignition 2331 |
| `MONNA.MOD` | monnaparty |
| `ONTHERUN.MOD` | on the run |
| `RUSHMORE.MOD` | rushmore remix |
| `TERMINAL.MOD` | terminal velocity |

## JGMOD 0.99

Author: Guan Foo Wah / Jeffery Guan Foo Wah. Source mirror: [ArtificialRaccoon/JGMOD](https://github.com/ArtificialRaccoon/JGMOD).

Vendored source is in `vendor/JGMOD-main/src`; the complete original guestware notice is in [vendor/JGMOD-main/readme.txt](vendor/JGMOD-main/readme.txt). It is included as `Licenses/JGMOD.txt` in binary packages. This library is not relicensed by the project's MIT grant. The library's historical registration link may no longer be available.

## Allegro and DJGPP

The original executable was linked with Allegro and the DJGPP runtime. Allegro fixed-point helpers, palette behavior, and lookup tables were recovered to preserve game behavior; these library-derived portions are attributed to Shawn Hargreaves and the Allegro contributors, not claimed as original game code. The LZSS format investigation used Allegro's implementation, credited there to Shawn Hargreaves and Haruhiko Okumura. See Allegro's published [giftware terms](https://liballeg.org/stabledocs/en/readme.html#copyright) and [license FAQ](https://liballeg.org/stabledocs/en/faq.html).

Library helper evidence in `analysis/typed` (fixed-point, random-number, and fade routines) and generated `src/trig_tables.h` are retained with their provenance. The modern application does not link the historical DOS runtime or Allegro library. DOS executables remain archival build inputs and are not included in native binary packages.

## SDL2, SDL2-compat, and SDL3

SDL is provided by the [SDL project](https://www.libsdl.org/), under the zlib license. Windows uses the official SDL 2.32.10 MinGW binary package. Homebrew Mac builds use SDL2-compat and SDL3. Linux builds use the distribution's SDL2 package.

Binary packages that bundle SDL libraries also include the corresponding vendor license text under `Licenses`. Linux packages normally rely on the system SDL installation rather than redistributing it. SDL2-compat is an API compatibility library, not a DOS or CPU emulator.

## Build and recovery tools

The Windows build downloads the official Zig 0.14.1 toolchain and SDL development archive with pinned SHA-256 hashes. Zig, Clang/LLVM, MinGW runtime components, and their included licenses remain those of their respective authors. Downloaded toolchains are not part of the source repository. Windows packages include the toolchain's license and available libc/libc++ runtime notices, because runtime code may be linked into the executable.

Ghidra and Temurin JDK were used during reverse engineering. Their downloaded distributions, caches, and project databases are excluded from the public source set and are unnecessary for ordinary builds. The recovery scripts and typed output are included.

## Preserved websites and contributed maps

`www/` retains historical HTML, images, downloads, credits, and provenance. Jacob Schwartz’s artwork uses the asset license above; third-party content retains its existing terms. `www/downloads/maps01.zip` is a user-contributed map pack and is not covered by Jacob Schwartz’s asset grant. The unchanged historical pages and manuals may describe older distribution terms; retain them as preservation records. Modern restoration code and documentation use MIT.
