# Changelog

All notable changes to the bHaptics C/C++ SDK in this repository. Each version matches `BHAPTICS_VERSION_*` in `bhaptics.h`, the CMake package version and a `v<version>` [release](https://github.com/bhaptics/tact-cpp2/releases).

## [2.7.1] - 2026-10-07

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
- `playWaveform`.
- The Visual Studio sample solution — Visual Studio opens the CMake project directly, and `sdk/README.md` covers manual project setup.

### Upgrading

Rebuild your application against the new header; replacing the DLL alone is not enough. A binary built against `library.h` that calls `reInitMessage` fails to load (entry point not found), and `resume` no longer returns a value.

### Checksums (SHA-256)

```
399992988dc899173e921be23409bed6dc8e7e0cbe78f505452f311fead06b26  sdk/lib/x64/bhaptics_library.dll
ff769c111f4c06bfd7b41925c8e428c88e1b415bb332212d3d3d8f0542396332  sdk/lib/x64/bhaptics_library.lib
ac556983bf429f7d23b9416111d73070bcce9806fb3730a5af1cc2e9a6f49b41  sdk/lib/x86/bhaptics_library.dll
845a4062133882ac66e092b6d114de4ed4678864318b24717a42f958ee038ca0  sdk/lib/x86/bhaptics_library.lib
0afb299e9d242e1c1745a56744708c305789edc0a80acdf6d4d45abbfa6d66c9  sdk/lib/libbhaptics_library.dylib
```

Check a copy with `Get-FileHash <file>` (PowerShell) or `shasum -a 256 <file>` (macOS).

[2.7.1]: https://github.com/bhaptics/tact-cpp2/releases/tag/v2.7.1
