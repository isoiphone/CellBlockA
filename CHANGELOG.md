# Changelog

## Unreleased

### Game and editor

- Recover Jail-ed 1.1b from the 1.3b distribution using the symbolized 1.0 reference, preserving editor tools, canvas, cursors, tile variants and start markers.
- Build native macOS and Windows editor applications; create, load, edit and save original CEL maps and PCX previews.
- Share user map storage with the game and decode packed maps at runtime; reject malformed input and invalid start points.
- Test editor round trips, tools, previews and game simulation under sanitizers, plus packaged editor-to-game integration.
- Build both applications in CI and collect platform/source archives and checksums through the release workflow. Native Windows runtime verification still requires a successful Windows CI run.
- Disable SDL text-entry mode in game/editor canvases so macOS does not treat gameplay keys as composed text. Physical-key controls and native file-dialog text fields remain available.
- Fix a 1.3b recovery error that mislabeled Allegro horizontal sprite mirroring as vertical flipping, corrupting platform pieces, player animations, directional particle sprites and backdrop sprites. Verify the helper against original machine code, regenerate the affected functions, and test actual rendered pixels across every tile kind and all player animation frames in both directions.
- Correct the reconstruction's target from 1.0 to **1.3b**, as identified by the executable and manual inside `cellin13.exe` (February 4, 2000).
- Recover all 64 game functions from the stripped 1.3b executable, using the symbolized 1.0 release as a research reference. Retain both distributions and their separate recovery evidence.
- Use all 469 GAME objects, 25 MENU objects and 12 maps from 1.3b, including DEATH, the intro/help screens, expanded player voices, changed death behavior and arena backdrop. Verify 8-/16-bit sound assets. Retain the approved map-boundary safety fixes.
- Validate developer replay input before indexing keyboard state; reject invalid keys, states, timestamps, and incomplete records.
- Recreate package output directories to exclude stale files, and verify native source exports in isolation with website links adapted only in the exported README.

### Native platform and release support

- Add standalone Jail-ed downloads for Windows and Linux alongside the macOS editor packages, with independent resources/notices and separate-package relocation tests in CI.

- Recover the original game's C/C++ logic and unpack its original Allegro assets and maps.
- Add a native SDL backend with original two-player controls, graphics and music files.
- Add macOS app packaging and a cross-compiled Windows x64 portable package.
- Fix out-of-map collision reads and out-of-map redraw writes with the original author's approval.
- Add a portable Mac/Linux build path, architecture-aware Mac packaging, and Linux packaging using system SDL2.
- Prevent overflow when converting high-resolution clock counters after long system uptimes; retain the original scheduled tick intervals.
- Add native test runners and CI for Windows, Ubuntu x64, and both Mac architectures.
- Add MIT code licensing and CC BY-SA 4.0 licensing for Jacob Schwartz's artwork and sound effects; explicitly exclude third-party MOD music and its embedded samples from both grants.
- Add third-party notices, public build/player/contributor documentation, and a clean source export.

### Repository preparation

- Remove local build outputs, downloaded recovery tools, unused vendor examples/binaries, and intermediate analysis files; retain reproducible inputs and historical preservation artifacts.
- Consolidate build, contributor, release, and licensing documentation. Author-owned game/editor code and tools use MIT; author-owned assets use CC BY-SA 4.0. Bundled MOD files and their licensing remain unchanged.

Known limits: exact DOS timing/audio equivalence is not established. Windows/Linux and Intel Mac runtime claims require successful testing on those platforms. See [FIDELITY.md](docs/FIDELITY.md).
