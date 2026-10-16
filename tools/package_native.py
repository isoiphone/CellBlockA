#!/usr/bin/env python3
"""Package a built Mac/Linux game; macOS bundles its non-system dylibs."""
import os
from pathlib import Path
import platform
import plistlib
import re
import shutil
import struct
import subprocess
import tarfile
from release_files import ROOT, copy_resources, copy_notices, fresh_directory, copy_editor_package


def output(*args):
    return subprocess.check_output([str(a) for a in args], text=True).strip()


def run(*args):
    subprocess.run([str(a) for a in args], check=True)


def dylibs(path):
    return [line.strip().split(' (compatibility version', 1)[0]
            for line in output('otool', '-L', path).splitlines()[1:]]


def minimum_versions(load_commands):
    """Read deployment versions, not LC_SOURCE_VERSION or SDK/library versions."""
    versions = []
    command = None
    for line in load_commands.splitlines():
        fields = line.split()
        if len(fields) >= 2 and fields[0] == 'cmd':
            command = fields[1]
        elif len(fields) == 2 and (
            (command == 'LC_BUILD_VERSION' and fields[0] == 'minos') or
            (command == 'LC_VERSION_MIN_MACOSX' and fields[0] == 'version')
        ):
            versions.append(tuple(int(v) for v in fields[1].split('.')))
    return versions


def package_mac(name="CellBlockA", display="Cell Block A"):
    app = ROOT / 'dist' / (display + '.app')
    fresh_directory(app)
    contents = app / 'Contents'
    frameworks = contents / 'Frameworks'
    resources = contents / 'Resources'
    exe = contents / 'MacOS' / name
    exe.parent.mkdir(parents=True, exist_ok=True)
    frameworks.mkdir(parents=True, exist_ok=True)
    shutil.copy2(ROOT / 'build' / name, exe)
    copy_resources(resources)
    copy_notices(resources)
    copied = {}

    def bundle(path, name=None):
        path = Path(path).resolve()
        if path in copied:
            return copied[path]
        target = frameworks / (name or path.name)
        if target.exists():
            target.chmod(0o755)
        shutil.copy2(path, target)
        target.chmod(0o755)
        copied[path] = target
        # The first entry of a dylib is its install ID, not a dependency.
        for dep in dylibs(path)[1:]:
            if dep.startswith(('/System/', '/usr/lib/')) or dep == str(path):
                continue
            if dep.startswith('/'):
                # A library's install ID may be an alternate symlink to itself.
                if Path(dep).resolve() == path:
                    continue
                child = bundle(dep)
                run('install_name_tool', '-change', dep, '@loader_path/' + child.name, target)
            else:
                raise RuntimeError('Unsupported dependency path (cannot ensure portability): ' + dep)
        run('install_name_tool', '-id', '@rpath/' + target.name, target)
        return target

    for dep in dylibs(exe):
        if dep.startswith(('/System/', '/usr/lib/')):
            continue
        if not dep.startswith('/'):
            raise RuntimeError('Expected absolute installed library path: ' + dep)
        target = bundle(dep)
        run('install_name_tool', '-change', dep,
            '@executable_path/../Frameworks/' + target.name, exe)
    # SDL2-compat loads this dynamically, so otool cannot discover it.
    sdl3 = ROOT / 'build/libSDL3.dylib'
    if sdl3.exists():
        bundle(sdl3, 'libSDL3.dylib')

    # Discover notices from the actual Homebrew installation, on either CPU.
    if shutil.which('brew'):
        for formula in ('sdl2-compat', 'sdl3', 'sdl2'):
            result = subprocess.run(['brew', '--prefix', formula], capture_output=True, text=True)
            if result.returncode == 0:
                license_file = Path(result.stdout.strip()) / 'LICENSE.txt'
                if license_file.exists():
                    shutil.copy2(license_file, resources / 'Licenses' / (formula + '.txt'))
    # Never claim a lower deployment target than a bundled library actually has.
    minimums = []
    for binary in [exe, *copied.values()]:
        minimums.extend(minimum_versions(output('otool', '-l', binary)))
    if not minimums:
        raise RuntimeError('Cannot determine the macOS deployment target')
    minimum = '.'.join(map(str, max(minimums)))
    png_path = ROOT / 'build/CellBlockA-icon.png'
    run('sips', '-s', 'format', 'png', ROOT / 'original-1.3b/CELL.ICO', '--out', png_path)
    png = png_path.read_bytes()
    chunk = b'icp5' + struct.pack('>I', len(png) + 8) + png
    (resources / 'CellBlockA.icns').write_bytes(b'icns' + struct.pack('>I', len(chunk) + 8) + chunk)
    info = dict(CFBundleExecutable=name, CFBundleIdentifier='com.kevlarx.' + name.lower(),
                CFBundleName=display, CFBundleDisplayName=display,
                CFBundlePackageType='APPL', CFBundleShortVersionString='1.1' if name == 'Jailed' else '1.3',
                CFBundleGetInfoString=('Jail-ed 1.1b — native port' if name == 'Jailed'
                                      else 'Cell Block A 1.3b — native reconstruction'),
                CFBundleVersion='1', NSHighResolutionCapable=True,
                LSMinimumSystemVersion=minimum, CFBundleIconFile='CellBlockA.icns')
    (contents / 'Info.plist').write_bytes(plistlib.dumps(info))
    shutil.copy2(ROOT / 'docs/PLAYING.md', resources / 'PLAYING.md')
    shutil.copy2(ROOT / 'docs/JAILED.md', resources / 'JAILED.md')
    for binary in frameworks.glob('*.dylib'):
        run('codesign', '--force', '--sign', '-', binary)
    run('codesign', '--force', '--sign', '-', app)
    run('codesign', '--verify', '--deep', '--strict', app)
    archive = ROOT / 'dist' / (name + '-macOS-' + platform.machine() + '.zip')
    run('ditto', '-c', '-k', '--keepParent', app, archive)
    print(f'Packaged {archive.name}; minimum macOS {minimum}')


def package_linux():
    name = 'CellBlockA-Linux-' + platform.machine()
    folder = ROOT / 'dist' / name
    fresh_directory(folder)
    shutil.copy2(ROOT / 'build/CellBlockA', folder / 'CellBlockA')
    shutil.copy2(ROOT / 'build/Jailed', folder / 'Jailed')
    shutil.copy2(ROOT / 'docs/JAILED.md', folder / 'JAILED.md')
    copy_resources(folder / 'resources')
    copy_notices(folder)
    shutil.copy2(ROOT / 'docs/PLAYING.md', folder / 'PLAYING.md')
    (folder / 'README.txt').write_text(
        'Run ./CellBlockA or ./Jailed in a desktop session. SDL2 and the system C++ runtime are required.\n'
        'Ubuntu/Debian: sudo apt install libsdl2-2.0-0 zenity\n'
        'Zenity provides editor file dialogs. See JAILED.md for editor controls.\n'
        'Build on the target distribution for best compatibility.\n'
        'See PLAYING.md, FIDELITY.md, LICENSE and THIRD_PARTY_NOTICES.md.\n', encoding='utf-8')
    archive = ROOT / 'dist' / (name + '.tar.gz')
    with tarfile.open(archive, 'w:gz') as tar:
        tar.add(folder, arcname=name)
    print('Packaged', archive)
    editor_name = 'Jailed-Linux-' + platform.machine()
    editor = ROOT / 'dist' / editor_name
    copy_editor_package(folder, editor, 'Jailed')
    editor_archive = ROOT / 'dist' / (editor_name + '.tar.gz')
    with tarfile.open(editor_archive, 'w:gz') as tar:
        tar.add(editor, arcname=editor_name)
    print('Packaged', editor_archive)


if __name__ == '__main__':
    system = platform.system()
    if system == 'Darwin':
        package_mac()
        package_mac("Jailed", "Jail-ed")
    elif system == 'Linux':
        package_linux()
    else:
        raise SystemExit('Use tools/build_windows.py on Windows')
