# Changelog

All notable changes to the bHaptics C/C++ SDK in this repository. Versions match `BHAPTICS_VERSION_*` in `bhaptics.h`.

## 2.7.1

### Added

- **Windows x86 (32-bit)** and **macOS (universal: Apple silicon + Intel)** libraries, next to Windows x64.
- **CMake package**: `find_package(bhaptics CONFIG REQUIRED PATHS sdk)` → `bhaptics::bhaptics`, picking x64 or x86 to match your build.
- **Examples**: `hello.c`, `play_events.cpp`, `motor_control.cpp`, built by the top-level `CMakeLists.txt`.
- `playWithStartTimeToDevice`, `playLoopToDevice`, `playDotToDevice`, `playPathToDevice` — drive one of several devices of the same type.
- `retryInitialize`, `pauseEvent`, `playWaveformDk3` (TactGlove DK3), `bHapticsShutdown`.
- `BHAPTICS_VERSION_MAJOR` / `_MINOR` / `_PATCH` macros.
- The header pins the calling convention (`__cdecl` on Windows), so projects built with `/Gz` or `/Gr` link, and is plain ASCII, so `/WX` builds on any code page stay clean.

### Changed

- Layout: the header is `sdk/include/bhaptics/bhaptics.h` (was `tact-cpp2/tact-cpp2/library.h`); libraries are under `sdk/lib/` (were in `lib/`).
- `resume` returns `void`.
- `bHapticsGetHapticMappings` / `bHapticsGetHapticMessage` take the status as `int32_t*` instead of `int&`.

### Removed

- `pause` — use `pauseEvent`.
- `reInitMessage` — use `retryInitialize`.
- The Visual Studio sample solution — Visual Studio opens the CMake project directly, and `sdk/README.md` covers manual project setup.
