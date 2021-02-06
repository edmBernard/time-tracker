# time-tracker [![C++ CI](https://github.com/edmBernard/time-tracker/workflows/C++%20CI/badge.svg?branch=master)](https://github.com/edmBernard/time-tracker/actions)
Minimalist command line tool to track time spend

## Dependencies

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

This project depends on:
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library
- [replxx](https://github.com/AmokHuginnsson/replxx): A small, portable GNU readline replacement for Linux, Windows and MacOS.

```
./vcpkg install cxxopts fmt replxx
```

### Embed dependencies:

I also use dependencies in thirdparty folder.

- [cxxsubs](https://github.com/edmBernard/cxxsubs): A Library to add Subcommand to cxxopts
- [data.h](https://github.com/HowardHinnant/date): A date and time library based on standard chrono header


## Build instructions

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

the server executable is named `timetracker`

## Disclaimer

It's a toy project. Mainly used to learn some new library.
Your comments are welcome, if you spot error or improvement.
