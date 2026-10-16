# Contributing

This project preserves Cell Block A's original mechanics, controls, graphics and sound assets. Focus changes on build portability, documented recovery corrections, stability, and demonstrated fidelity issues. Discuss intentional gameplay or presentation changes before implementing them.

## Working on the code

1. Follow [BUILDING.md](docs/BUILDING.md) for your platform.
2. Keep `original-1.3b/` (current build inputs) and `original/` (1.0 reference) byte-for-byte unchanged. The build checks both SHA-256 manifests.
3. Make recovered-code adaptations in `tools/generate_native.py`; `src/recovered.c`, `src/native.h`, `src/asset_types.h`, and `src/trig_tables.h` are generated. Backend code lives in `src/platform.cpp`; globals in `src/state.c`.
4. Keep current typed decompiler evidence in `analysis/typed-1.3b` separate from deliberate port fixes; `analysis/typed` preserves the earlier 1.0 research. Cite original addresses or behavior when proposing a recovery correction.
5. Add focused regression tests for behavior or memory-safety fixes. Run the platform tests and `python3 tools/check_repo.py`, then package and smoke-test relevant releases.
6. Describe what changed, why, how it was tested, and what remains unverified. Do not claim Windows/Linux verification from cross-compilation alone.

Builds generate shared files, so use separate checkouts for simultaneous builds. Do not include `build/`, `dist/`, downloaded toolchains, Ghidra databases, settings, or local crash logs in pull requests. Preserve third-party source notices. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for license scope.

## Reporting problems

Include OS/version, CPU architecture, build command or package name, map, keys/actions, expected/actual behavior, and a minimal reproduction. Relevant sanitizer or test logs help. For graphics/fidelity issues, attach a screenshot or reference recording and identify its source. Remove personal paths or unrelated information from logs before posting.

The two approved map-boundary safety corrections are intentional differences from the DOS program; do not restore their out-of-bounds behavior.

Contributions to the MIT-licensed portion should be offered under the same MIT license. Jacob Schwartz's artwork, maps, and sound effects use [CC BY-SA 4.0](ASSET_LICENSE.md); shared adaptations must comply with its attribution and ShareAlike terms. MOD music and its embedded samples are excluded from both project licenses. Do not add assets, samples, or music without clear provenance and permission.

## Editor changes

Keep `analysis/jailed-1.3b/` as recovery evidence. Change reproducible adaptations in `tools/generate_jailed.py`, host/editor services in `src/jailed.cpp`, and shared serialization in `src/map_io.cpp`. Run the sanitizer suite and `tools/test_jailed.py` after changes affecting map files, editor tools or game discovery. Preserve original tools, canvas and asset selection; document intentional platform differences in `docs/FIDELITY.md`.
