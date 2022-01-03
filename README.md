# Print Center Fix [![Download](https://badgen.net/github/release/Hun1eR/Print-Center-Fix)](https://github.com/Hun1eR/Print-Center-Fix/releases/latest) ![Platform](https://badgen.net/badge/platform/linux%20%7C%20windows/gray) [![License](https://img.shields.io/github/license/Hun1eR/Print-Center-Fix)](LICENSE)


This Metamod plugin fixes messages output on old (non-steam) CS 1.6 clients («%s» instead of a message in the center) caused by AMXX Mod X build v5260 and later.


<details>
  <summary><b>Screenshots</b></summary>
  <p align="center">
    <img src="https://i.imgur.com/jBTk4o7.png" alt="BUG">
    <img src="https://i.imgur.com/7oQCX7E.png" alt="FIX">
  </p>
</details>


## Installation
* Install the [Metamod](http://metamod.org)
* Download archive [printcenterfix.zip](https://github.com/Hun1eR/Print-Center-Fix/releases/latest)
* Unpack the `printcenterfix_mm.dll` (for Windows) or `printcenterfix_mm_i386.so` (for Linux) to `<hlds>/cstrike/addons/printcenterfix` directory.
* Edit the file `<hlds>/cstrike/addons/metamod/plugins.ini`
* For Windows add:
```ini
  win32 addons\printcenterfix\printcenterfix_mm.dll
```
* For Linux add:
```ini
  linux addons/printcenterfix/printcenterfix_mm_i386.so
```


## Building
#### Prerequisites:
&nbsp;&nbsp;&nbsp;Compiler with C++17 support, [CMake](https://cmake.org) version 3.21 or later, [Ninja](https://ninja-build.org/).
#### Build with MSVC (Windows):
```sh
  cmake --preset windows-msvc-release && cmake --build --preset windows-msvc-release
```
#### Build with Clang (Windows):
```sh
  cmake --preset windows-clang-release && cmake --build --preset windows-clang-release
```
#### Build with GCC (Linux):
```sh
  cmake --preset linux-gcc-release && cmake --build --preset linux-gcc-release
```
#### Build with Clang (Linux):
```sh
  cmake --preset linux-clang-release && cmake --build --preset linux-clang-release
```
#### Build with Intel C++ Compiler (Linux):
```sh
  cmake --preset linux-intel-release && cmake --build --preset linux-intel-release
```


## Credits
Kubo Takehiro ([funchook](https://github.com/kubo/funchook))

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
