"""Shared resource and notice staging. Only named game inputs are accessed."""
from pathlib import Path
import json
import shutil

ROOT = Path(__file__).resolve().parents[1]


def fresh_directory(destination):
    """Reset a generated package directory so old files cannot enter a release."""
    destination = Path(destination)
    if destination.is_symlink():
        raise RuntimeError('Refusing to replace a symlink: ' + str(destination))
    if destination.exists():
        shutil.rmtree(destination)
    destination.mkdir(parents=True)


def copy_resources(destination):
    destination = Path(destination)
    # Copy only indexed assets and recorded originals, never arbitrary local
    # extraction/debug files that happen to sit beside them.
    files = []
    for archive in ('GAME', 'MENU'):
        base = Path('assets') / archive
        index = ROOT / 'analysis' / base / 'index.json'
        entries = json.loads(index.read_text(encoding='utf-8'))
        files.append((index, base / 'index.json'))
        for number, entry in enumerate(entries):
            name = f'{number:03d}.bin'
            if entry['file'] != name:
                raise RuntimeError('Unexpected extracted asset filename: ' + entry['file'])
            files.append((ROOT / 'analysis' / base / name, base / name))
    manifest = json.loads((ROOT / 'analysis/original-1.3b-sha256.json').read_text(encoding='utf-8'))
    for name in manifest:
        if name.startswith(('MAPS/', 'MUSIC/')) or name in ('CELL.CFG', 'README.TXT', 'GAME.PAL', 'CELL.ICO'):
            files.append((ROOT / 'original-1.3b' / name, Path('original') / name))
        if name.startswith('MAPS/') and name.endswith('.CEL'):
            relative = Path('maps') / (Path(name).stem + '.bin')
            files.append((ROOT / 'analysis' / relative, relative))
    for source, relative in files:
        target = destination / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)


def copy_notices(destination):
    destination = Path(destination)
    destination.mkdir(parents=True, exist_ok=True)
    for name in ('LICENSE', 'ASSET_LICENSE.md', 'THIRD_PARTY_NOTICES.md'):
        shutil.copy2(ROOT / name, destination / name)
    shutil.copy2(ROOT / 'docs/FIDELITY.md', destination / 'FIDELITY.md')
    shutil.copy2(ROOT / 'docs/RECOVERY.md', destination / 'RECOVERY.md')
    licenses = destination / 'Licenses'
    licenses.mkdir(exist_ok=True)
    shutil.copy2(ROOT / 'vendor/JGMOD-main/readme.txt', licenses / 'JGMOD.txt')


def copy_editor_package(source, destination, executable):
    """Stage a standalone editor from a complete, freshly built portable package."""
    source, destination = Path(source), Path(destination)
    fresh_directory(destination)
    files = [executable, 'LICENSE', 'ASSET_LICENSE.md', 'THIRD_PARTY_NOTICES.md',
             'FIDELITY.md', 'RECOVERY.md', 'JAILED.md']
    if executable.endswith('.exe'):
        files.append('SDL2.dll')
    for name in files:
        shutil.copy2(source / name, destination / name)
    for name in ('resources', 'Licenses'):
        shutil.copytree(source / name, destination / name)
    launch = ('Run Jailed.exe. Keep SDL2.dll and resources beside it.\n'
              if executable.endswith('.exe') else
              'Run ./Jailed in a desktop session.\n'
              'Ubuntu/Debian: sudo apt install libsdl2-2.0-0 zenity\n'
              'SDL2 and the system C++ runtime are required; Zenity provides file dialogs.\n')
    (destination / 'README.txt').write_text(
        'Jail-ed 1.1b — native map editor\n\n' + launch +
        'Extract the entire archive before running. No game installation is required.\n'
        'Install Cell Block A separately to play maps; both programs share the user map folder.\n'
        'See JAILED.md for controls and map storage, and LICENSE, ASSET_LICENSE.md,\n'
        'and THIRD_PARTY_NOTICES.md for license scope.\n', encoding='utf-8')
