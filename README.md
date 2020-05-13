# Print Center Fix [![Build Status](https://travis-ci.com/Hun1eR/Print-Center-Fix.svg?branch=master)](https://travis-ci.com/Hun1eR/Print-Center-Fix) [![Download](https://badgen.net/github/release/Hun1eR/Print-Center-Fix)](https://github.com/Hun1eR/Print-Center-Fix/releases/latest) ![Platform](https://badgen.net/badge/platform/linux%20%7C%20windows/gray) [![License](https://img.shields.io/github/license/Hun1eR/Print-Center-Fix)](LICENSE)


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
Compiler with C++17 support ([Visual Studio](https://visualstudio.microsoft.com), [GCC](https://gcc.gnu.org) or [Intel C++ Compiler](https://software.intel.com/c-compilers)).  
[CMake](https://cmake.org) version 3.13.5 or later (for Linux).

#### Building on Windows:
&nbsp;&nbsp;&nbsp;&nbsp;Just open the solution in Visual Studio and compile.

#### Building on Linux:
&nbsp;&nbsp;&nbsp;Open the console and go to the directory with sources (where *CMakeLists.txt* is located).  
&nbsp;&nbsp;&nbsp;For compile with *GCC* run the following command:<br/>
```sh
  rm -rf build && CC=gcc CXX=g++ cmake -B build && cmake --build build --parallel
```
&nbsp;&nbsp;&nbsp;For compile with *Intel C++ Compiler* run the following command:<br/>
```sh
  rm -rf build && CC=icc CXX=icpc cmake -B build && cmake --build build --parallel
```

## Release History
* 1.0.1
    * Updated SDKs.
    * Added some additional optimization flags for the Linux version.
* 1.0.0
    * First release.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
