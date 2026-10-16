#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
cc="${CC:-clang}"
cxx="${CXX:-clang++}"
python="${PYTHON:-python3}"
sdl_config="${SDL2_CONFIG:-sdl2-config}"
for command in "$cc" "$cxx" "$python" "$sdl_config"; do
  if ! command -v "$command" >/dev/null; then
    echo "Missing build dependency: $command. See docs/BUILDING.md." >&2
    exit 1
  fi
done
mkdir -p build/obj
"$python" tools/extract_original.py
"$python" tools/generate_native.py
"$python" tools/generate_jailed.py
flags=(-O1 -g -fsigned-char -fwrapv -fno-strict-aliasing -Isrc)
if [[ "${SANITIZE:-0}" == 1 ]]; then flags+=(-fsanitize=address,undefined -fno-omit-frame-pointer); fi
if [[ "${TEST_BUILD:-0}" == 1 ]]; then flags+=(-DCELL_TEST_BUILD); fi
read -r -a sdl_cflags <<< "$("$sdl_config" --cflags)"
read -r -a sdl_libs <<< "$("$sdl_config" --libs)"
"$cc" -std=c11 "${flags[@]}" -Wno-parentheses -Wno-division-by-zero -c src/recovered.c -o build/obj/recovered.o
"$cc" -std=c11 "${flags[@]}" -c src/state.c -o build/obj/state.o
"$cxx" -std=c++17 "${flags[@]}" -Ivendor/JGMOD-main/src "${sdl_cflags[@]}" -c src/platform.cpp -o build/obj/platform.o
for name in file_io load_mod load_s3m load_xm load_it load_jgm save_jgm mod player player2 player3 player4; do
  "$cc" -std=c11 "${flags[@]}" -Wno-invalid-source-encoding -Wno-pointer-sign -Ivendor/JGMOD-main/src -c "vendor/JGMOD-main/src/$name.c" -o "build/obj/jg_$name.o"
done
"$cxx" -std=c++17 "${flags[@]}" -c src/map_io.cpp -o build/obj/map_io.o
"$cc" -std=c11 "${flags[@]}" -Wno-parentheses -c src/jailed_recovered.c -o build/obj/jailed_recovered.o
objects=(build/obj/map_io.o build/obj/jailed_recovered.o build/obj/recovered.o build/obj/state.o build/obj/platform.o build/obj/jg_*.o)
if [[ "${TEST_BUILD:-0}" == 1 ]]; then
  "$cxx" -std=c++17 "${flags[@]}" -c tests/core.cpp -o build/obj/core.o
  "$cxx" -std=c++17 "${flags[@]}" -c tests/jailed.cpp -o build/obj/jailed_tests.o
  objects+=(build/obj/core.o build/obj/jailed_tests.o)
fi
"$cxx" "${flags[@]}" "${objects[@]}" "${sdl_libs[@]}" -o build/CellBlockA
"$cxx" -std=c++17 "${flags[@]}" -DJAILED_APP -Ivendor/JGMOD-main/src "${sdl_cflags[@]}" -c src/platform.cpp -o build/obj/jailed_platform.o
"$cxx" -std=c++17 "${flags[@]}" "${sdl_cflags[@]}" -c src/jailed.cpp -o build/obj/jailed.o
dialog_flags=()
dialog_libs=()
if [[ "$(uname -s)" == Darwin ]]; then dialog_flags=(-x objective-c++); dialog_libs=(-framework Cocoa); fi
"$cxx" -std=c++17 "${flags[@]}" "${dialog_flags[@]}" -c src/native_dialog.cpp -o build/obj/native_dialog.o
editor_objects=()
for object in "${objects[@]}"; do
  if [[ "$object" != build/obj/platform.o ]]; then editor_objects+=("$object"); fi
done
"$cxx" "${flags[@]}" "${editor_objects[@]}" build/obj/jailed_platform.o build/obj/jailed.o build/obj/native_dialog.o "${sdl_libs[@]}" "${dialog_libs[@]}" -o build/Jailed
# SDL2-compat dynamically loads SDL3. Discover the installed Homebrew prefix
# instead of assuming Apple Silicon's /opt/homebrew (Intel uses /usr/local).
if [[ "$(uname -s)" == Darwin ]]; then
  sdl3_prefix="${SDL3_PREFIX:-}"
  if [[ -z "$sdl3_prefix" ]] && command -v brew >/dev/null; then
    sdl3_prefix="$(brew --prefix sdl3 2>/dev/null || true)"
  fi
  if [[ -n "$sdl3_prefix" && -f "$sdl3_prefix/lib/libSDL3.0.dylib" ]]; then
    if [[ -e build/libSDL3.dylib ]]; then chmod u+w build/libSDL3.dylib; fi
    cp "$sdl3_prefix/lib/libSDL3.0.dylib" build/libSDL3.dylib
  fi
fi
if [[ ! -e analysis/original ]]; then ln -s ../original-1.3b analysis/original; fi
printf 'Built native executables: build/CellBlockA and build/Jailed (%s, %s)\n' "$(uname -s)" "$(uname -m)"
