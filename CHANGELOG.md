# Changelog

All notable changes to the bHaptics C/C++ SDK in this repository. Each version matches `BHAPTICS_VERSION_*` in `bhaptics.h`, the CMake package version and a `v<version>` [release](https://github.com/bhaptics/tact-cpp2/releases).

## [2.8.2] - 2026-10-21

### Added

- `bHapticsGetVersion()` — the version of the loaded library as `"MAJOR.MINOR.PATCH"`, to compare with the `BHAPTICS_VERSION_*` macros. On Windows the DLL now also has a version resource (Properties → Details).
- `playWaveform` is declared in the header again. The 2.7.1 libraries exported it, but the header left it out.
- macOS: `isPlayerInstalled`, `isPlayerRunning` and `launchPlayer` are implemented; they always returned `false` before.

### Changed

- `registryAndInit` / `registryAndInitHost` start the connection in the background and return at once, instead of blocking the calling thread (for up to about 2 s while the Player is not running). Wait for `wsIsConnected()` — and for `getEventTime()` of your event — before the first play, as the examples do.
- `wsIsConnected()` is a single atomic read: no lock, no allocation. Cheap enough to call every frame.
- `isPlayerRunning()` on Windows is faster: it no longer scans the whole process list.

### Fixed

- `isbHapticsConnected()` reported every device as disconnected with current bHaptics Player versions.

### Upgrading

No function signature changed since 2.7.1, so existing builds keep working with the new libraries; rebuild against the new header to use `bHapticsGetVersion` and `playWaveform`. If your code played right after `registryAndInit` returned, wait for `wsIsConnected()` first.

### Checksums (SHA-256)

```
dd6c79fd65ed4ab2b9d980a8813a7b5d577fb4d81b055d1d9360d966f368960a  sdk/lib/x64/bhaptics_library.dll
5ee56fd75d179c04b5d544b9f90058d623630799479c9cf87e8570b8d260b2a1  sdk/lib/x64/bhaptics_library.lib
fd7f6da872f1b590a8e1346adb62d5da028cfbe469bdf0b70f0b97d97bda012a  sdk/lib/x86/bhaptics_library.dll
002b98db73803d61430aba2fb0c781771f6eab5e4e28f6e0d51a0072822b60b2  sdk/lib/x86/bhaptics_library.lib
13eed18b8cd2628ac3d258346321afcceea10e9d7db5482830e41008584f89bd  sdk/lib/libbhaptics_library.dylib
```

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

[2.8.2]: https://github.com/bhaptics/tact-cpp2/releases/tag/v2.8.2
[2.7.1]: https://github.com/bhaptics/tact-cpp2/releases/tag/v2.7.1
