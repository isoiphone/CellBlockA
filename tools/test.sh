#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
SANITIZE=1 TEST_BUILD=1 tools/build.sh
mkdir -p build/test-save
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 \
CELL_SAVE_DIR="$PWD/build/test-save" CELL_VERIFY=1 \
  build/CellBlockA 2>&1 | tee build/core-test.log
