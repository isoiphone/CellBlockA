#!/usr/bin/env python3
"""Headless native Mac/Linux package smoke test, including relocated resources."""
import argparse
import os
from pathlib import Path
import platform
import shutil
import subprocess
from release_files import ROOT, fresh_directory
from replay_checks import check_invalid_replays


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('package', type=Path, help='Built .app or Linux package folder')
    args = parser.parse_args()
    source = args.package.resolve()
    test_root = ROOT / 'build/package-test'
    destination = test_root / 'path with spaces' / source.name
    fresh_directory(destination)
    shutil.copytree(source, destination, dirs_exist_ok=True)
    executable = (destination / 'Contents/MacOS/CellBlockA'
                  if platform.system() == 'Darwin' else destination / 'CellBlockA')
    capture = test_root / 'gameplay.bmp'
    if capture.exists():
        capture.unlink()
    env = dict(os.environ, SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy',
               CELL_SAVE_DIR=str(test_root / 'save'),
               CELL_INPUT_SCRIPT=str(ROOT / 'tests/gameplay.input'),
               CELL_TEST_SECONDS='19', CELL_TEST_CAPTURE=str(capture))
    env.pop('CELL_RESOURCE_DIR', None)
    env.pop('CELL_VERIFY', None)
    result = subprocess.run([str(executable)], cwd=test_root, env=env,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=90)
    (test_root / 'smoke-test.log').write_bytes(result.stdout)
    print(result.stdout.decode('utf-8', errors='replace'), end='')
    result.check_returncode()
    if not capture.exists() or capture.stat().st_size < 640 * 480 * 3:
        raise RuntimeError('Packaged game did not produce its framebuffer capture')
    if b'Native replay complete:' not in result.stdout or b'ticks=0\n' in result.stdout:
        raise RuntimeError('Packaged game did not reach an active match')
    print('PASS relocated native package gameplay smoke test')
    check_invalid_replays(executable, test_root, env)


if __name__ == '__main__':
    main()
