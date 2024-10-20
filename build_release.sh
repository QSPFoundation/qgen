#!/bin/sh

export QGEN_RELEASE_VER="${1:-0.0.0}"

mkdir -p dist

echo "Preparing Win32 release $QGEN_RELEASE_VER"
./build_packages/pack_win32.sh

echo "Preparing Linux64 release $QGEN_RELEASE_VER"
./build_packages/pack_linux64.sh

echo "Preparing Linux64 AppImage release $QGEN_RELEASE_VER"
./build_packages/pack_linux64_AppImage.sh
