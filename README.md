# CPPND: Capstone MiniHTTPD

This is a web server supporting http/1.0 and http/1.1 written in C++.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac) 
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
* cl & msbuild (Windows)
  * Windows: recommend using [MSVC 2019](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)

## Basic Build Instructions

## Linux, Mac
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./minihttpd`.

## Windows
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && msbuild minihttpd.sln /p:Configuration=Release`
4. Run it: `Release\minihttpd.exe`.
