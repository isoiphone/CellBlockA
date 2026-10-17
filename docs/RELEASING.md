# Preparing a public release

Cell Block A and Jail-ed ship as native applications for macOS, Windows, and Linux. This guide covers source validation, the first GitHub push, and publishing downloadable packages.

The [first native preview](https://github.com/isoiphone/CellBlockA/releases/tag/native-v1.3.0-preview.1) contains the game/editor binaries and source built from `99c0fb3`, with `SHA256SUMS.txt`. Its release notes record platform requirements and the successful CI runs.

Author-owned code uses [MIT](../LICENSE); author-owned assets use [CC BY-SA 4.0](../ASSET_LICENSE.md). Bundled MOD music and embedded samples keep their existing terms and are excluded from both grants. Preserve all [third-party notices](../THIRD_PARTY_NOTICES.md). The outstanding rights and platform-verification records are listed below.

## What belongs in Git

Keep native source, build/test/recovery tools, saved typed recovery evidence, original distributions and hash manifests, vendored JGMOD source/notices, and documentation. The preserved websites under `www/` and historical DOS ZIPs under `archives/` are intentional archival inputs; keep their recorded bytes unchanged.

Generated native packages belong in release attachments. Keep `build/`, `dist/`, downloaded toolchains, Ghidra databases, extracted assets, caches, and local settings out of Git. See [BUILDING.md](BUILDING.md) for dependencies and generated-file details.

## Validate the source and packages

Run from the full repository root. On Windows, substitute `py -3` for `python3`:

```sh
python3 tools/check_repo.py
python3 tools/test_source.py
python3 www/_archive/restore.py --check
python3 tools/archive_original.py --check
python3 tools/archive_original.py --version 1.3b --check
python3 tools/extract_release13.py
```

On macOS or Linux, build both applications and run the sanitizer suite before packaging:

```sh
./tools/test.sh
./tools/package.sh
```

Then test the packages on the same platform. On macOS:

```sh
python3 tools/test_package.py 'dist/Cell Block A.app'
python3 tools/test_jailed.py --editor 'dist/Jail-ed.app/Contents/MacOS/Jailed' --game 'dist/Cell Block A.app/Contents/MacOS/CellBlockA'
```

On Linux:

```sh
python3 tools/test_package.py "dist/CellBlockA-Linux-$(uname -m)"
python3 tools/test_jailed.py --editor "dist/Jailed-Linux-$(uname -m)/Jailed" --game "dist/CellBlockA-Linux-$(uname -m)/CellBlockA"
```

On Windows, this command builds, packages, and tests both programs, including the standalone editor:

```powershell
py -3 tools/build_windows.py --test
```

The Windows script can also cross-compile on macOS without `--test`; that does not verify Windows runtime behavior. Automated tests use headless SDL. Check real windows, audio, keyboard input, editor mouse tools, and save/open dialogs on every platform advertised as verified.

`tools/test_source.py` exports `dist/CellBlockA-source.tar.gz`, verifies its complete hash manifest, and runs repository checks in an isolated directory. It does not compile the exported source. For a clean-build check, extract it elsewhere and run that platform's build/tests there. The source archive includes both unpacked original distributions, but omits the preserved website and convenience DOS ZIPs; the preservation-only commands above require the full repository.

## Keep one initial commit before the first push

The initial publication uses one parentless commit on `main`. Before pushing, verify:

```sh
git status --short
git rev-list --count main
git log --all --oneline
```

The status output should be empty, the count should be `1`, and the log should show only `Initial commit`. If you make further pre-publication fixes, stage the intended files and use `git commit --amend --no-edit` to keep that single commit. Repeat affected checks before publishing. After publication, use normal commits for subsequent development rather than rewriting published history.

Amending changes the commit ID. Old commits may remain in local reflogs for recovery; pushing `main` sends only its reachable history. Do not use `--mirror` or `--all` for the initial push: those options can publish unrelated local refs or branches.

## Create the GitHub repository and push

1. Open [GitHub's new repository page](https://github.com/new), choose the owner, name (for example `CellBlockA`), and visibility.
2. Leave README, `.gitignore`, and license initialization disabled. Create an empty repository; these files already exist locally.
3. In a terminal at this project's root, substitute your account/repository name and run:

```sh
git remote add origin https://github.com/YOUR-USERNAME/CellBlockA.git
git remote -v
git push -u origin main
```

No `git init` or force push is needed. If `origin` already exists, inspect `git remote -v` and change it with `git remote set-url origin <URL>` only if necessary. For an SSH remote, use `git@github.com:YOUR-USERNAME/CellBlockA.git` instead. HTTPS uses your configured credential helper or a personal access token; SSH uses your configured key. GitHub account passwords do not authenticate Git pushes. See GitHub's [existing-code guide](https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github).

After pushing, confirm GitHub shows one commit on `main`. Open **Actions** and inspect **Native Mac and Linux builds**, **Windows native build**, and **Historical preservation checks**. The first push runs these workflows; it does not create a GitHub Release or deploy the website.

## Build and publish release downloads

1. Finalize the changelog, version metadata, and [fidelity record](FIDELITY.md) before building the release. Commit those changes, following the pre-publication rule above if the first push has not happened yet.
2. On the exact commit to release, run **Build all release artifacts** through **Actions → Run workflow**, or push a release tag matching `native-v*`. This workflow calls the native and Windows workflows, then checks the complete artifact set and generates `SHA256SUMS.txt`. It does not run the separate preservation workflow; confirm that check also passed for the release commit.
3. Download the workflow's **release-files** artifact and extract its outer ZIP. It contains the nine archives below plus `SHA256SUMS.txt`. Verify the checksums with `shasum -a 256 -c SHA256SUMS.txt` on macOS or `sha256sum -c SHA256SUMS.txt` on Linux. On Windows, compare `Get-FileHash <archive> -Algorithm SHA256` with the manifest.
4. Test the downloaded packages on their target OS/CPU. Windows and Linux game packages include both programs; standalone Jail-ed packages include the editor's resources and notices. Linux needs the system SDL2 runtime and Zenity for editor file dialogs. macOS packages have separate game/editor apps and architecture-specific minimum OS requirements.
5. Create a GitHub Release using a tag pointing to that tested commit. Attach the nine inner archives and checksum file from the successful workflow. State tested platforms, minimum OS versions, known limitations, macOS ad-hoc signing/notarization status, Windows signing status, and Linux runtime dependencies. Include the license scope and unchanged music terms.

| Platform | Game download | Standalone editor download |
| --- | --- | --- |
| Windows x64 | `CellBlockA-Windows-x64.zip` | `Jailed-Windows-x64.zip` |
| macOS Apple Silicon | `CellBlockA-macOS-arm64.zip` | `Jailed-macOS-arm64.zip` |
| macOS Intel | `CellBlockA-macOS-x86_64.zip` | `Jailed-macOS-x86_64.zip` |
| Linux x64 | `CellBlockA-Linux-x86_64.tar.gz` | `Jailed-Linux-x86_64.tar.gz` |

The ninth archive is `CellBlockA-source.tar.gz`, containing source for both programs. Local Linux builds use the host architecture in archive names; CI currently builds Linux x64. Artifact collection uses read-only repository permissions and does not publish releases automatically. CI artifacts and locally generated source archives are not signed release attestations.

## Outstanding release gates

- **Third-party music:** the supplied distribution has incomplete per-track rights records. Record redistribution terms or permission for each MOD track and its embedded samples before treating the complete bundle as cleared for redistribution. The files and their licensing remain unchanged; exclusion from MIT and CC BY-SA does not establish permission.
- **Vocals:** the 1.3b manual credits Thomas Craft and David Bronough separately from Jacob Schwartz's sound effects. Preserve those credits and establish the scope of redistribution rights for those contributions.
- **JGMOD:** its retained guestware notice requests registration at a historical GeoCities URL. Record how that condition has been satisfied or obtain clarification from its author. The library is not covered by the project's MIT grant.
- **Platform verification:** inspect successful CI runs and perform real window/audio/input testing on Windows, Linux, and Intel Mac before advertising those platforms as verified. Cross-compilation and package-layout tests alone are insufficient.
