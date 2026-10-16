# Cell Block A preserved websites

Open [`index.html`](index.html) in a browser to choose a version. Both versions work directly from disk or on any static HTTP server, without a build, JavaScript, or package installation. Keep this entire `www` folder together: the two versions share downloads and preservation records.

| Folder | Restored design | Details |
| --- | --- | --- |
| [`1999/`](1999/index.html) | Green on black, animated geometric navigation | [Capture dates and substitutions](1999/restoration.html) |
| [`2001/`](2001/index.html) | Blue background, gray text, horizontal image buttons | [Capture dates and substitutions](2001/restoration.html) |

Each version contains Main, Screens, Download, and Links pages. These are composite restorations of `www.netidea.com/cell` using surviving Internet Archive captures, not complete single-date or pixel-perfect snapshots. The selector and restoration notes are modern additions; the historical wording, notices, layouts, and surviving artwork are retained.

## Shared downloads and evidence

- `downloads/cellin13.exe`: unchanged version 1.3 installer from the October 24, 2001 capture. Its SHA-1 matches the CDX digest. Downloaded, not executed. Its 39 extracted files are preserved in `original-1.3b/` and verified by `tools/extract_release13.py`; `original/` holds the separate 1.0 reference distribution.
- `downloads/maps01.zip`: unchanged user-created map pack from June 3, 2001. ZIP CRC verification passes; it contains 33 `.cel` maps and 34 `.pcx` previews. It has not been installed into the reconstructed game. All 2001 installer mirrors point to the shared local installer; their historical labels remain.
- `_archive/original/`: unchanged Wayback `id_` replay payloads for the earlier site and shared artwork. The four later HTML sources are under `_archive/original/2001/`. These source files intentionally retain original missing/remote references; browse the restored year folders instead.
- `_archive/manifest.json`: original URLs, capture dates, byte sizes, SHA-256 hashes, and CDX SHA-1 comparisons. Digest mismatches are explicitly recorded. The map ZIP's CDX query failed with HTTP 503; its date is verified by the saved replay headers, without claiming a CDX digest match.
- `_archive/cdx-*.json` and `_archive/headers/`: archived inventories and selected response metadata, with cookies removed.
- `_archive/restore.py`: deterministic offline restoration and validation, using Python 3.9+ and its standard library.

## Rebuild and verify

From the repository root:

```sh
python3 www/_archive/restore.py          # rebuild both versions' historical pages and assets
python3 www/_archive/restore.py --check  # verify without modifying files
```

Verification checks every saved source/download SHA-256, exact generated page/asset bytes, and local `href`, `src`, and `background` references across both versions, their notes, and the selector. It does not contact historical external sites or execute the installer. The selector, notes, README, and manifest are maintained preservation records, not generated historical content.

Validation after reorganization passed for 43 source/download hashes, 45 generated files, and 137 local references across 11 pages. All four 2001 HTML payloads match their CDX digests. Browser review covered the four 2001 pages (all 23 image elements loaded), the selector, and the moved 1999 homepage/download page. Both shared downloads returned HTTP 200 in the local preview. The 2001 gallery's width beyond the header is inherited from the original tables, not a responsive redesign.

For HTTP preview, run `python3 -m http.server 8000 --directory www` and visit `http://localhost:8000/`.

## Fidelity and rights

The 1999 restoration substitutes text for three missing graphics, reuses two surviving alternate navigation icons, and omits the CGI counter. The 2001 restoration leaves the original charcoal table fill in place of three missing decorative header strips, reuses two alternate button states, and omits its remote visitor counter. The 2001 font requests remain, with monospace fallbacks where OCR A Extended and Terminal are unavailable. Fonts are not bundled. Both versions use later/earlier surviving image captures where needed, as detailed in their notes.

Original authorship remains KevlarX / Flaming Head Software. Jacob Schwartz’s artwork is covered by the repository’s [CC BY-SA 4.0 asset grant](../ASSET_LICENSE.md). Modern restoration code and documentation use [MIT](../LICENSE). Third-party material, including the contributed map pack and MOD music, retains its existing terms; see [license scope and notices](../THIRD_PARTY_NOTICES.md). Original copyright notices and manuals remain unchanged as historical records. External links and contact details are historical. Creating these files does not publish the repository or website.
