# QGen

## Linux build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Windows build

```bash
mkdir build
cd build
cmake -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=out ..
cmake --build . --target install --config Release
```

## TODO

* Add filtering for shown locations
* Fix initial window sizes
* Fix installation into a local directory (Linux)
* Build web editor

## Chat group

https://discord.gg/6gWVYUtUGZ
