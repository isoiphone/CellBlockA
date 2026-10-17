#!/usr/bin/env python3
"""Build native Windows x64 C/C++ binaries, on Windows or macOS.

Requires Python 3.9+. Pinned compiler/SDL archives are downloaded from their
official sites, verified with SHA-256, and cached only inside build/.
"""
import argparse
import hashlib
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile
from release_files import copy_resources, copy_notices, fresh_directory, copy_editor_package

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build' / 'windows'
DEPS = ROOT / 'build' / 'windows-deps'
DOWNLOADS = ROOT / 'build' / 'downloads'
DIST = ROOT / 'dist' / 'CellBlockA-Windows-x64'
EDITOR_DIST = ROOT / 'dist' / 'Jailed-Windows-x64'
ZIG_VERSION = '0.14.1'
ZIG_HASHES = {
    'aarch64-macos': '39f3dc5e79c22088ce878edc821dedb4ca5a1cd9f5ef915e9b3cc3053e8faefa',
    'x86_64-macos': 'b0f8bdfb9035783db58dd6c19d7dea89892acc3814421853e5752fe4573e5f43',
    'x86_64-windows': '554f5378228923ffd558eac35e21af020c73789d87afeabf4bfd16f2e6feed2c',
}
SDL_ARCHIVE = 'SDL2-devel-2.32.10-mingw.tar.gz'
SDL_HASH = '83a5d74012311edc3c0d40ea6faecbe57ad692aa033fa5dc273cc937e3938ff2'
JG_SOURCES = ('file_io load_mod load_s3m load_xm load_it load_jgm save_jgm '
              'mod player player2 player3 player4').split()


def fetch(url, digest, extracted_name):
    DOWNLOADS.mkdir(parents=True, exist_ok=True)
    archive = DOWNLOADS / url.rsplit('/', 1)[1]
    if not archive.exists():
        print('Downloading', url, flush=True)
        partial = archive.with_suffix(archive.suffix + '.part')
        urllib.request.urlretrieve(url, partial)
        partial.replace(archive)
    if hashlib.sha256(archive.read_bytes()).hexdigest() != digest:
        raise RuntimeError('SHA-256 mismatch: ' + str(archive))
    dest = DEPS / extracted_name
    if not dest.exists():
        DEPS.mkdir(parents=True, exist_ok=True)
        # Only these pinned, hash-verified vendor archives are extracted.
        if archive.suffix == '.zip':
            with zipfile.ZipFile(archive) as f:
                f.extractall(DEPS)
        else:
            with tarfile.open(archive) as f:
                f.extractall(DEPS)
    if not dest.is_dir():
        raise RuntimeError('Missing extracted dependency: ' + str(dest))
    return dest


def run(args, **kwargs):
    subprocess.run([str(a) for a in args], cwd=ROOT, check=True, **kwargs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--test', action='store_true', help='Also run native tests (Windows host required)')
    args = parser.parse_args()
    system = {'Darwin': 'macos', 'Windows': 'windows'}.get(platform.system())
    arch = {'arm64': 'aarch64', 'aarch64': 'aarch64', 'amd64': 'x86_64',
            'x86_64': 'x86_64'}.get(platform.machine().lower())
    host = f'{arch}-{system}'
    if host not in ZIG_HASHES:
        parser.error('Supported build hosts: Windows x64, macOS arm64/x64')
    if args.test and system != 'windows':
        parser.error('--test runs Windows executables; use it on Windows')
    extension = '.zip' if system == 'windows' else '.tar.xz'
    zig_name = f'zig-{host}-{ZIG_VERSION}'
    zig_dir = fetch(f'https://ziglang.org/download/{ZIG_VERSION}/{zig_name}{extension}',
                    ZIG_HASHES[host], zig_name)
    zig = zig_dir / ('zig.exe' if system == 'windows' else 'zig')
    sdl_dir = fetch('https://www.libsdl.org/release/' + SDL_ARCHIVE,
                    SDL_HASH, 'SDL2-2.32.10')
    sdl = sdl_dir / 'x86_64-w64-mingw32'
    obj_dir = BUILD / 'obj'
    obj_dir.mkdir(parents=True, exist_ok=True)
    fresh_directory(DIST)
    os.environ['ZIG_GLOBAL_CACHE_DIR'] = str(ROOT / 'build' / 'zig-cache')
    os.environ['ZIG_LOCAL_CACHE_DIR'] = str(BUILD / 'zig-cache')
    run([sys.executable, 'tools/extract_original.py'])
    run([sys.executable, 'tools/generate_native.py'])
    run([sys.executable, 'tools/generate_jailed.py'])
    # DWARF keeps native crash stacks readable with the CI runner's GDB.
    flags = ['-target', 'x86_64-windows-gnu', '-O1', '-gdwarf', '-fsigned-char',
             '-fwrapv', '-fno-strict-aliasing', '-Isrc', '-Ivendor/JGMOD-main/src',
             '-I' + str(sdl / 'include' / 'SDL2')]

    def compile_source(source, name, test=False, editor=False):
        cpp = str(source).endswith('.cpp')
        output = obj_dir / (name + '.o')
        # Zig's optimized modes define NDEBUG; the regression suite requires
        # assert expressions (including setup calls) to execute.
        extra = (['-DCELL_TEST_BUILD', '-UNDEBUG'] if test else []) + (['-DJAILED_APP'] if editor else [])
        print('Compiling', source, flush=True)
        run([zig, 'c++' if cpp else 'cc', '-std=c++17' if cpp else '-std=c11',
             *flags, *extra, '-Wno-parentheses', '-Wno-pointer-sign',
             '-Wno-invalid-source-encoding', '-c', source, '-o', output])
        return output

    common = [compile_source('src/recovered.c', 'recovered'),
              compile_source('src/state.c', 'state'),
              compile_source('src/map_io.cpp', 'map_io'),
              compile_source('src/jailed_recovered.c', 'jailed_recovered')]
    for name in JG_SOURCES:
        common.append(compile_source(f'vendor/JGMOD-main/src/{name}.c', 'jg_' + name))
    host_object = compile_source('src/platform.cpp', 'platform')
    test_host = compile_source('src/platform.cpp', 'platform-test', True)
    tests = compile_source('tests/core.cpp', 'core', test=True)
    editor_tests = compile_source('tests/jailed.cpp', 'jailed-tests', test=True)
    editor_host = compile_source('src/platform.cpp', 'jailed-platform', editor=True)
    editor = compile_source('src/jailed.cpp', 'jailed')
    dialogs = compile_source('src/native_dialog.cpp', 'native-dialog')
    # SDL's Windows entry point handles the GUI subsystem and UTF-8 argv.
    # Explicit import library: Zig otherwise prefers SDL's static archive.
    link = ['-lmingw32', sdl / 'lib' / 'libSDL2main.a', sdl / 'lib' / 'libSDL2.dll.a']
    run([zig, 'c++', *flags, *common, host_object, *link, '-Wl,--subsystem,windows',
         '-o', DIST / 'CellBlockA.exe'])
    run([zig, 'c++', *flags, *common, test_host, tests, editor_tests, *link,
         '-o', BUILD / 'CellBlockA-tests.exe'])
    run([zig, 'c++', *flags, *common, editor_host, editor, dialogs, *link, '-lcomdlg32',
         '-Wl,--subsystem,windows', '-o', DIST / 'Jailed.exe'])
    for destination in (BUILD, DIST):
        shutil.copy2(sdl / 'bin' / 'SDL2.dll', destination / 'SDL2.dll')
    copy_resources(DIST / 'resources')
    copy_notices(DIST)
    shutil.copy2(ROOT / 'docs/JAILED.md', DIST / 'JAILED.md')
    licenses = DIST / 'Licenses'
    licenses.mkdir(exist_ok=True)
    shutil.copy2(sdl_dir / 'LICENSE.txt', licenses / 'SDL2.txt')
    shutil.copy2(ROOT / 'vendor/JGMOD-main/readme.txt', licenses / 'JGMOD.txt')
    for source, name in (
        ('LICENSE', 'Zig.txt'),
        ('lib/libc/mingw/COPYING', 'MinGW-w64.txt'),
        ('lib/libcxx/LICENSE.TXT', 'libcxx.txt'),
        ('lib/libcxxabi/LICENSE.TXT', 'libcxxabi.txt'),
        ('lib/libunwind/LICENSE.TXT', 'libunwind.txt'),
    ):
        shutil.copy2(zig_dir / source, licenses / name)
    shutil.copy2(ROOT / 'docs/WINDOWS.md', DIST / 'README.md')
    shutil.copy2(ROOT / 'docs/FIDELITY.md', DIST / 'FIDELITY.md')
    copy_editor_package(DIST, EDITOR_DIST, 'Jailed.exe')
    if args.test:
        run([sys.executable, 'tools/test_windows.py'])
    archive = shutil.make_archive(str(DIST), 'zip', DIST.parent, DIST.name)
    editor_archive = shutil.make_archive(str(EDITOR_DIST), 'zip', EDITOR_DIST.parent, EDITOR_DIST.name)
    print('Built native Windows x64 applications:', DIST / 'CellBlockA.exe', EDITOR_DIST / 'Jailed.exe')
    print('Portable ZIP:', archive)
    print('Standalone editor ZIP:', editor_archive)
    if system != 'windows':
        print('Cross-compiled only. Windows runtime testing requires a Windows host.')


if __name__ == '__main__':
    main()
