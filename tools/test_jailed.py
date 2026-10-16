#!/usr/bin/env python3
"""Exercise packaged editor create/edit/save/load and play its map in the game."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
from extract_original import unpack
from release_files import ROOT


def relocate_executable(executable, destination):
    """Relocate each app independently so editor-only packages work too."""
    if executable.parent.name == 'MacOS':
        destination = destination.with_suffix('.app')
        shutil.copytree(executable.parents[2], destination)
        return destination / 'Contents/MacOS' / executable.name
    if executable.suffix.lower() == '.exe' or (executable.parent / 'resources').is_dir():
        shutil.copytree(executable.parent, destination)
        return destination / executable.name
    return executable


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--editor', required=True, type=Path)
    parser.add_argument('--game', required=True, type=Path)
    args = parser.parse_args()
    editor, game = args.editor.resolve(), args.game.resolve()
    test_root = ROOT / 'build/jailed-package-test'
    test_root.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='native caf\u00e9 \u6f22\u5b57 ', dir=test_root) as temp:
        work = Path(temp)
        save = work / 'save'
        maps = save / 'maps'
        maps.mkdir(parents=True)
        new, edited = maps / '000NEW.CEL', maps / '000EDIT.CEL'
        env = dict(os.environ, SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy',
                   CELL_SAVE_DIR=str(save), ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                   UBSAN_OPTIONS='halt_on_error=1')
        for key in ('CELL_RESOURCE_DIR', 'CELL_VERIFY', 'CELL_INPUT_SCRIPT', 'CELL_TEST_SECONDS', 'CELL_TEST_CAPTURE'):
            env.pop(key, None)
        # For packaged inputs relocate the whole app/folder, including resources.
        editor = relocate_executable(editor, work / 'editor package')
        game = relocate_executable(game, work / 'game package')
        commands = ['new', 'tool e'] + [f'click {x*32} 96 1' for x in range(20)]
        commands += ['tool w', 'click 0 96 1', 'tool r', 'click 608 96 1',
                     f'save {new}', 'new', f'load {new}', 'tool q', 'click 320 64 1',
                     f'save {edited}', 'new', f'load {edited}', f'capture {work / "editor.pcx"}']
        script = work / 'edit.script'
        script.write_text('\n'.join(commands) + '\n', encoding='utf-8')
        result = subprocess.run([str(editor), '--script', str(script)], cwd=work, env=env,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=45)
        (test_root / 'editor.log').write_bytes(result.stdout)
        result.check_returncode()
        a, b = unpack(new.read_bytes()), unpack(edited.read_bytes())
        assert len(a) == len(b) == 272
        assert a[10*13+2] == 0 and b[10*13+2] == 4
        assert sum(x != y for x, y in zip(a, b)) == 1
        from struct import unpack_from
        for path in (new, edited):
            pcx = path.with_suffix('.pcx').read_bytes()
            assert pcx[:4] == bytes((10, 5, 1, 8))
            assert unpack_from('<HH', pcx, 8) == (199, 149)
            assert pcx[-769] == 12
        # The game selects alphabetically first 000EDIT through its normal menu.
        env.update(CELL_INPUT_SCRIPT=str(ROOT / 'tests/gameplay.input'),
                   CELL_TEST_SECONDS='19', CELL_TEST_CAPTURE=str(test_root / 'gameplay.bmp'))
        result = subprocess.run([str(game)], cwd=work, env=env, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, timeout=45)
        (test_root / 'game.log').write_bytes(result.stdout)
        print(result.stdout.decode('utf-8', errors='replace'), end='')
        result.check_returncode()
        assert b'map=maps/000EDIT.CEL;' in result.stdout, 'Game did not play the editor-created map'
        assert b'ticks=0\n' not in result.stdout
        assert (test_root / 'gameplay.bmp').stat().st_size >= 640*480*3
        shutil.copy2(work / 'editor.pcx', test_root / 'editor.pcx')
    print('PASS native editor create/save/load/edit/save, independent map/PCX decode, Unicode relocation, and actual game menu/match')


if __name__ == '__main__':
    main()
