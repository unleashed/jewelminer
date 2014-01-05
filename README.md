# Jewel Miner - a PoC of typical board puzzle games

Licensed under the GNU General Public License version 3. See COPYING for
details.

![](jewelminer.gif?raw=true)

## Hierarchy of directories:

/bin - Makefile will output binaries and copy libraries here
/dist - Precompiled Windows x64 compressed package
/include - The game's C++ header files
/src - The game's C++ source code
/win64 - Libraries extracted from MinGW64 runtime and SDKs for SDL and SDL_image
/vendor - The original SDK's for MinGW64 for SDL 2.0.3 and SDL_image 2.0.0

## Compatibility

The game has been built successfully with the following C++11 compilers:

  - GNU C++ Compiler (g++) 4.8.1 on Linux AMD64
  - LLVM's Clang++ 3.4 using libc++ on Linux AMD64
  - MinGW64's GNU C++ (g++) 4.8.1 on Windows 7 x64

* and the following library versions:

  - SDL 2.0.0 (Linux AMD64), SDL 2.0.3 (Windows x64),
  - SDL_image 2.0.0 on both Linux AMD64 and Windows 7 x64
  - (with up-to-date library dependencies)

## Build instructions

### Requirements

* Linux / Unix-like:

SDL 2.0.X and SDL_image 2.0.X development packages. Either LLVM Clang++ 3.4 or
GNU G++ 4.8, or (possibly) later versions.

* Windows

MinGW64 from the MinGWbuilds project, located at
http://sourceforge.net/projects/mingwbuilds.

The version tested was installed via the automated install, choosing the
x64-4.8.1-release-win32-seh-rev5 version. Note that the libraries under
win64/mingw64 come from this version, and they may very well not work correctly
if you use any other release. The fix would be to use your compiler's libraries,
ie. libgcc_s*.dll and libstdc++6.dll.

### Compilation

* Linux / Unix-like:

Run "make gcc" or "make clang" to compile and output the binary as
bin/jewelminer.

* Windows:

Run "make win" _under a mingw64 command prompt_. Such prompt is available once you
install via mingwbuild installer.

## Running the game

* Linux:

Either run "./run.sh" or execute jewelminer from the bin directory. This is
because the binary will look for resources in "../res" from your current working
directory.

* Windows:

Either run "run.bat" or execute jewelminer from the bin directory. See Linux for
reasons. Alternatively you could just double-click jewelminer.

### Command line

The game supports specifying the number of columns and rows if you run it like:

jewelminer 16 16
