[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

```
git clone
git submodule update --init --recursive
```

### Install

- c++ compiler (MSVC, gcc, clang)
- cmake e.g. `choco install cmake`
- ninja e.g. `choco install ninja`
- engine: `cd thirdparty\gameengine && thirdparty\vcpkg\vcpkg.exe install @vcpkg_x64-windows.txt && cd ../../`

### Building (CMake)

I develop with visual studio code. You can open the .code-workspace using the C/C++ extention tools:

    C/C++ by Microsoft
    This extension provides IntelliSense support for C/C++.
    VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools

    CMake Tools by vector-of-bool
    This extension allows for configuring the CMake project and building it from within the Visual Studio Code IDE.
    VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=vector-of-bool.cmake-tools

### Emscripten

(run once)

```
thirdparty/gameengine/thirdparty/emsdk/emsdk install latest
```

Look in the [./.vscode/tasks.json](./.vscode/tasks.json) for emscripten build commands
