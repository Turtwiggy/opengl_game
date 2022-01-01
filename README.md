[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

```
git clone
git submodule update --init --recursive
```

```
(Windows) Building with g++ via mingw64

(run as much as needed to rebuild)
make -j 14
```

```
(Emscripten)

(run once)
thirdparty/gameengine/thirdparty/emsdk/emsdk install latest

(run once in current terminal)
thirdparty/gameengine/thirdparty/emsdk/emsdk activate latest

(run as much as needed to rebuild)
cmake -j 14 EMSCRIPTEN=1
```
