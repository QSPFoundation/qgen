#!/bin/sh

set -e

# Validation
[ ! -d "./build_packages" ] && echo "Run this script from the project root directory" && exit
[ -z "$RELEASE_VER" ] && echo "RELEASE_VER isn't specified" && exit

# Build
mkdir -p ./build_packages/win32

REL_BUILD_DIR=./build_packages/win32

./build_packages/dockcross-windows-static-x86 cmake -S . -B $REL_BUILD_DIR -GNinja \
  -DAPP_VERSION="$RELEASE_VER" \
  -DCPACK_OUTPUT_FILE_PREFIX=$REL_BUILD_DIR/packages \
  -DCPACK_GENERATOR="ZIP;NSIS" \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DCMAKE_BUILD_TYPE=Release

./build_packages/dockcross-windows-static-x86 cmake --build $REL_BUILD_DIR --parallel $(nproc)
./build_packages/dockcross-windows-static-x86 cpack -B $REL_BUILD_DIR --config $REL_BUILD_DIR/CPackConfig.cmake

# Move to dist
mv $REL_BUILD_DIR/packages/*.zip "./dist/qgen-$RELEASE_VER-win32.zip"
mv $REL_BUILD_DIR/packages/*.exe "./dist/qgen-$RELEASE_VER-win32.exe"
