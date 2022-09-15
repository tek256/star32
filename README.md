<div id="header">
    <p align="center">
      <b>star32</b><br>
  	  <span font-size="16px">a cross platform fantasy console</span><br>
      <span font-size="12px">Made by <a href="http://tek256.com">Devon</a> with love.</span><br><br>
      <span><a href="https://github.com/tek256/star32/wiki/Build-Guide">Setup</a> | <a href="https://discordapp.com/invite/63GvpMh">Discord</a> | <a href="https://github.com/sponsors/tek256">Support</a></span><br><br>
      <!-- <span><img src="https://github.com/tek256/star32/workflows/Build%20Star32/badge.svg"></span> -->
    </p>
</div>

### About
star32 is a cross platform fantasy console aimed at running on a base model raspberry pi zero.

**Note: Currently this repository is under development, if you're reading this, it's before official release.**

### Features
```
- Simple to use scripting api (lua based)
- Cross platform support
- Free
```

### Target Platforms
Here are the target platforms for this library:  
```
- Windows
- Linux
- Mac OSX 
- BSD
``` 

_NOTE: I'm always open to adding more target platforms!_

### Building
star32 and it's dependencies are built using [CMake](https://cmake.org). You can run your own CMake command or try using one of the automated build scripts (they're located in the `tools/` folder).

```
Usage build_unix.sh && build_win.bat [-hrcxqq]
-h  Show this info
-r  Build release (optimizations, -O2)
-c  Build clean (remove previous build generated by this script)
-x  Don't build examples
-q  Quiet output
-qq Silence all output
Windows Only:
-m  Force use MinGW (gcc/g++)
-l  Force use LLVM (clang/clang++)
```

Example script usage (generate a release candidate):

```
./build_unix.sh -r
```

Example CMake usage:

```
cmake -Bbuild -S. 
cmake --build build
```

NOTE: Once the build files are generated (first line) you only have to call `cmake --build build` to rebuild the source.

For more information see the relevant [wiki page](https://github.com/tek256/star32/wiki/Build-Guide).
