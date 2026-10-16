#!/usr/bin/env python3
"""Run the compiled native Windows tests and portable-package smoke test."""
import os
from pathlib import Path
import shutil
import subprocess
import sys
from release_files import fresh_directory
from replay_checks import check_invalid_replays

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build' / 'windows'
DIST = ROOT / 'dist' / 'CellBlockA-Windows-x64'


def main():
    if os.name != 'nt':
        raise SystemExit('Run this script on Windows; no emulator is used.')
    env = dict(os.environ, SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy',
               CELL_SAVE_DIR=str(BUILD / 'test-save'))
    env.update(CELL_VERIFY='1', CELL_RESOURCE_DIR=str(DIST / 'resources'))
    result = subprocess.run([str(BUILD / 'CellBlockA-tests.exe')], env=env,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            timeout=120)
    (BUILD / 'core-test.log').write_bytes(result.stdout)
    print(result.stdout.decode('utf-8', errors='replace'), end='')
    result.check_returncode()
    del env['CELL_VERIFY']
    del env['CELL_RESOURCE_DIR']
    # Resources must be located relative to the executable, not the shell cwd.
    # SDL's UTF-8 path must survive a Windows UTF-16 filesystem location.
    relocated = BUILD / 'portable path caf\u00e9 \u6f22\u5b57'
    fresh_directory(relocated)
    shutil.copytree(DIST, relocated, dirs_exist_ok=True)
    capture = BUILD / 'gameplay.bmp'
    if capture.exists():
        capture.unlink()
    env.update(CELL_INPUT_SCRIPT=str(ROOT / 'tests/gameplay.input'),
               CELL_TEST_SECONDS='19', CELL_TEST_CAPTURE=str(capture))
    result = subprocess.run([str(relocated / 'CellBlockA.exe')], cwd=BUILD,
                            env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            timeout=45)
    (BUILD / 'smoke-test.log').write_bytes(result.stdout)
    print(result.stdout.decode('utf-8', errors='replace'), end='')
    result.check_returncode()
    if not capture.exists() or capture.stat().st_size < 640 * 480 * 3:
        raise RuntimeError('Native game did not produce its framebuffer capture')
    if b'Native replay complete:' not in result.stdout or b'ticks=0\n' in result.stdout:
        raise RuntimeError('Native game did not reach an active match')
    print('PASS native Windows core tests and relocated Unicode-path gameplay smoke test')
    check_invalid_replays(relocated / 'CellBlockA.exe', BUILD, env)
    subprocess.run([sys.executable, str(ROOT / 'tools/test_jailed.py'),
                    '--editor', str(ROOT / 'dist/Jailed-Windows-x64/Jailed.exe'),
                    '--game', str(DIST / 'CellBlockA.exe')], check=True)


if __name__ == '__main__':
    main()
