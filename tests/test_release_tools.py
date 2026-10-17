"""Regression checks for platform packaging metadata."""
from pathlib import Path
import sys
import unittest
import tempfile
import json
import os
import tarfile
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from package_native import minimum_versions
from release_files import fresh_directory
import export_source
import release_files
import package_native
from test_jailed import relocate_executable
from extract_original import coff, address_bytes


class RecoveryIdentity(unittest.TestCase):
    def test_sprite_flip_label_matches_original_machine_code(self):
        root=Path(__file__).resolve().parents[1]
        old,symbols=coff((root/'original/CELL.EXE').read_bytes())
        current,_=coff((root/'original-1.3b/CELL.EXE').read_bytes())
        names={s['name']:s['address'] for s in symbols}
        metadata=json.loads((root/'analysis/recovery-1.3b.json').read_text())
        address=metadata['symbols']['_draw_sprite_h_flip']
        # Entire 29-byte wrapper, including the distinguishing vtable offset.
        expected=bytes.fromhex('8b4424048b501cff742410ff742410ff742410508b4230ffd083c410c3')
        self.assertEqual(address_bytes(old,names['_draw_sprite_h_flip'],len(expected)),expected)
        self.assertEqual(address_bytes(current,address,len(expected)),expected)
        self.assertNotEqual(address_bytes(old,names['_draw_sprite_v_flip'],len(expected)),expected)
        self.assertNotIn('_draw_sprite_v_flip',metadata['symbols'])
        for name in ('_draw_player','_draw_tile','_play_game','_draw_points'):
            source=(root/'analysis/typed-1.3b'/f'{name}.c').read_text()
            self.assertIn('_draw_sprite_h_flip(',source)
            self.assertNotIn('_draw_sprite_v_flip(',source)


class ReleaseIsolation(unittest.TestCase):
    def test_standalone_editor_is_self_contained_and_relocates_separately(self):
        for extension in ('', '.exe'):
            with self.subTest(extension=extension), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                game, editor = root / 'game', root / 'editor'
                game.mkdir()
                for name in ('CellBlockA' + extension, 'Jailed' + extension, 'SDL2.dll',
                             'LICENSE', 'ASSET_LICENSE.md', 'THIRD_PARTY_NOTICES.md',
                             'FIDELITY.md', 'RECOVERY.md', 'JAILED.md', 'private-debug.txt'):
                    (game / name).write_bytes(name.encode())
                (game / ('Jailed' + extension)).chmod(0o755)
                for folder in ('resources', 'Licenses'):
                    (game / folder).mkdir()
                    (game / folder / 'required.dat').write_bytes(b'required dependency')
                release_files.copy_editor_package(game, editor, 'Jailed' + extension)
                moved_editor = relocate_executable(editor / ('Jailed' + extension), root / 'editor caf\u00e9')
                moved_game = relocate_executable(game / ('CellBlockA' + extension), root / 'game caf\u00e9')
                self.assertEqual(moved_editor.read_bytes(), ('Jailed' + extension).encode())
                self.assertEqual(moved_game.read_bytes(), ('CellBlockA' + extension).encode())
                self.assertFalse((moved_editor.parent / ('CellBlockA' + extension)).exists())
                self.assertFalse((moved_editor.parent / 'private-debug.txt').exists())
                if os.name != 'nt':
                    self.assertTrue(moved_editor.stat().st_mode & 0o111)
                for folder in ('resources', 'Licenses'):
                    self.assertEqual((moved_editor.parent / folder / 'required.dat').read_bytes(), b'required dependency')
                self.assertEqual((editor / 'SDL2.dll').exists(), extension == '.exe')
                self.assertIn('No game installation is required.', (editor / 'README.txt').read_text())

    @unittest.skipIf(os.name == 'nt', 'Unix executable modes require a Unix filesystem')
    def test_linux_archives_include_a_runnable_standalone_editor_layout(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for name in ('CellBlockA', 'Jailed'):
                executable = root / 'build' / name
                executable.parent.mkdir(exist_ok=True)
                executable.write_bytes(b'fixture executable')
                executable.chmod(0o755)
            for name in ('JAILED.md', 'PLAYING.md', 'FIDELITY.md', 'RECOVERY.md'):
                doc = root / 'docs' / name
                doc.parent.mkdir(exist_ok=True)
                doc.write_text('Fixture documentation')
            for name in ('LICENSE', 'ASSET_LICENSE.md', 'THIRD_PARTY_NOTICES.md',
                         'vendor/JGMOD-main/readme.txt'):
                notice = root / name
                notice.parent.mkdir(parents=True, exist_ok=True)
                notice.write_text('Fixture notice')
            def resources(destination):
                destination.mkdir(parents=True)
                (destination / 'asset.dat').write_bytes(b'fixture asset')
            with patch.object(package_native, 'ROOT', root), patch.object(release_files, 'ROOT', root), \
                    patch.object(package_native, 'copy_resources', resources), \
                    patch.object(package_native.platform, 'machine', return_value='x86_64'):
                package_native.package_linux()
            with tarfile.open(root / 'dist/Jailed-Linux-x86_64.tar.gz') as archive:
                prefix = 'Jailed-Linux-x86_64/'
                self.assertTrue(archive.getmember(prefix + 'Jailed').mode & 0o111)
                self.assertNotIn(prefix + 'CellBlockA', archive.getnames())
                for name in ('resources/asset.dat', 'Licenses/JGMOD.txt', 'LICENSE',
                             'ASSET_LICENSE.md', 'THIRD_PARTY_NOTICES.md', 'JAILED.md', 'README.txt'):
                    self.assertTrue(archive.getmember(prefix + name).isfile())
            with tarfile.open(root / 'dist/CellBlockA-Linux-x86_64.tar.gz') as archive:
                for name in ('CellBlockA', 'Jailed'):
                    self.assertTrue(archive.getmember('CellBlockA-Linux-x86_64/' + name).mode & 0o111)

    def test_unlisted_extraction_files_do_not_enter_package(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for name in ('GAME', 'MENU'):
                folder = root / 'analysis/assets' / name
                folder.mkdir(parents=True)
                (folder / 'index.json').write_text(json.dumps([{'file': '000.bin'}]))
                (folder / '000.bin').write_bytes(b'asset')
                (folder / 'private-debug.txt').write_text('must not ship')
            (root / 'analysis/original-1.3b-sha256.json').write_text('{}')
            with patch.object(release_files, 'ROOT', root):
                release_files.copy_resources(root / 'package')
            shipped = {p.relative_to(root / 'package').as_posix()
                       for p in (root / 'package').rglob('*') if p.is_file()}
            self.assertEqual(shipped, {'assets/GAME/000.bin', 'assets/GAME/index.json',
                                       'assets/MENU/000.bin', 'assets/MENU/index.json'})

    def test_package_reset_drops_stale_files_but_preserves_neighbor(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            neighbor = root / 'other-release.txt'
            neighbor.write_text('keep')
            package = root / 'package'
            fresh_directory(package)
            (package / 'stale-library.dll').write_bytes(b'old')
            fresh_directory(package)
            self.assertEqual(list(package.iterdir()), [])
            self.assertEqual(neighbor.read_text(), 'keep')

    def test_exported_readme_has_no_missing_website_links(self):
        for newline in ('\n', '\r\n'):
            with self.subTest(newline=repr(newline)):
                self.check_exported_readme(newline)

    def check_exported_readme(self, newline):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            readme = root / 'README.md'
            text = '# Game\n\nSee [site](www/index.html) and [DOS ZIP](archives/CellBlockA-original.zip).\n\nSee [build](docs/BUILDING.md).\n'
            original = text.replace('\n', newline).encode('utf-8')
            readme.write_bytes(original)
            with patch.object(export_source, 'ROOT', root):
                exported = export_source.export_bytes(readme).decode('utf-8')
            self.assertNotIn('](www/', exported)
            self.assertNotIn('](archives/', exported)
            self.assertIn('](docs/BUILDING.md)', exported)
            self.assertIn('not included', exported)
            self.assertEqual(readme.read_bytes(), original)


class MacDeploymentTarget(unittest.TestCase):
    def test_source_sdk_and_library_versions_are_not_deployment_versions(self):
        self.assertEqual(minimum_versions('''
             cmd LC_SOURCE_VERSION
         version 27037.1
             cmd LC_BUILD_VERSION
           minos 14.2
             sdk 27.0
             cmd LC_ID_DYLIB
 current version 401.14.0
'''), [(14, 2)])

    def test_legacy_deployment_command(self):
        self.assertEqual(minimum_versions('''
             cmd LC_VERSION_MIN_MACOSX
         version 10.15
             sdk 14.0
             cmd LC_SOURCE_VERSION
         version 9000.0
'''), [(10, 15)])

    def test_no_deployment_target(self):
        self.assertEqual(minimum_versions('cmd LC_SOURCE_VERSION\nversion 27037.1'), [])


if __name__ == '__main__':
    unittest.main()
