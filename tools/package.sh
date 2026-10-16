#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
SANITIZE=0 TEST_BUILD=0 tools/build.sh
"${PYTHON:-python3}" tools/package_native.py
