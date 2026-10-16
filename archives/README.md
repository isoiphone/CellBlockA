# Historical DOS game archives

- [CellBlockA-1.3b.zip](CellBlockA-1.3b.zip): all 39 files extracted from `cellin13.exe`, preserved in `original-1.3b/`. The executable and manual identify the release as **1.3b**, with the manual dated February 4, 2000. This is the native reconstruction's current source release. See [its SHA-256 manifest](../analysis/original-1.3b-sha256.json).
- [CellBlockA-original.zip](CellBlockA-original.zip): all 41 supplied **1.0** files from `original/`, including the DOS game, Jail-Ed, CWSDPMI and assets. This earlier distribution is retained as a research reference. See [its SHA-256 manifest](../analysis/original-sha256.json).

These are newly assembled convenience ZIPs, **not original download archives**. Their fixed ZIP timestamps are packaging metadata. The file bytes are unchanged. Unpacked copies remain in the repository for build/research use. These are archival DOS programs, not native Mac/Windows/Linux packages; archiving and verification do not execute them.

The website preserves [the original installer and a user-created map pack](../www/README.md) with provenance. Installer extraction only uppercases member filenames for the native DOS-compatible resource lookup; it does not alter their contents.

Verify both ZIPs and the installer:

```sh
python3 tools/archive_original.py --check
python3 tools/archive_original.py --version 1.3b --check
python3 tools/extract_release13.py
```

Omit `--check` to recreate a convenience ZIP. The default version remains 1.0 for compatibility with the earlier archival command; use `--version 1.3b` for the current release.

The original manuals and notices are retained. The repository's [code and asset license scope](../THIRD_PARTY_NOTICES.md) still applies; bundling files does not grant new rights to third-party contributions, MOD music, embedded samples or libraries. These archival ZIPs are intentionally tracked in Git, unlike generated native packages under `dist/`.
