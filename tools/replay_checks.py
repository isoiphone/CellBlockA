"""Regression checks for rejected developer replay input."""
import subprocess


def check_invalid_replays(executable, directory, environment):
    script = directory / 'invalid-replay.input'
    cases = ('0 -1 1', '0 128 1', '0 2147483647 1', '0 28 2',
             '18446744073709551615 28 1', '0 28', 'not a replay',
             '10 28 1\n9 28 0')
    for case in cases:
        script.write_text(case + '\n', encoding='utf-8')
        env = dict(environment, CELL_INPUT_SCRIPT=str(script))
        env.pop('CELL_VERIFY', None)
        result = subprocess.run([str(executable)], cwd=directory, env=env,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=15)
        if result.returncode != 1 or b'Invalid replay input:' not in result.stdout:
            raise RuntimeError(f'Replay was not safely rejected: {case!r}; '
                               f'exit={result.returncode}; output={result.stdout!r}')
    print(f'PASS {len(cases)} malformed replay inputs rejected before gameplay')
