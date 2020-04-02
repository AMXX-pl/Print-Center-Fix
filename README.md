# Print Center Fix [![Build Status](https://travis-ci.com/Hun1eR/Print-Center-Fix.svg?branch=master)](https://travis-ci.com/Hun1eR/Print-Center-Fix) [![Download](https://badgen.net/github/release/Hun1eR/Print-Center-Fix)](https://github.com/Hun1eR/Print-Center-Fix/releases/latest) ![Platform](https://badgen.net/badge/platform/linux%20%7C%20windows/gray) [![License](https://img.shields.io/github/license/Hun1eR/Print-Center-Fix)](https://github.com/Hun1eR/Print-Center-Fix/blob/master/LICENSE)
This Metamod plugin fixes messages output on old (non-steam) CS 1.6 clients («%s» instead of a message in the center) caused by AMXX Mod X build v5260 and higher.
<p align="center">
  <img src="https://i.imgur.com/VkAiZ3J.jpg">
</p>

# Building
#### Requirements:
* Compiler with C++17 support (Visual Studio, GCC or Intel C++ Compiler).
* CMake version 3.13.5 or higher (linux).

#### Building on Windows:
* Just open the solution in Visual Studio and compile.
#### Building on Linux:
* Open the console and go to the directory with sources (where *CMakeLists.txt* is located).
* For compile with *GCC* run the following command:<br/>
`rm -rf build && CC=gcc CXX=g++ cmake -B build && cmake --build build --parallel`
* For compile with *Intel C++ Compiler* run the following command:<br/>
`rm -rf build && CC=icc CXX=icpc cmake -B build && cmake --build build --parallel`
