# bHaptics C/C++ SDK

Drive bHaptics haptic devices — TactSuit, TactGlove, TactVisor, TactSleeve and more — from C and C++ through the [bHaptics Player](https://www.bhaptics.com/software/player/?type=pcplayer).

| Platform | Architectures | Library |
|---|---|---|
| Windows 10 / 11 | x64, x86 (32-bit) | `sdk/lib/x64/`, `sdk/lib/x86/` — `bhaptics_library.dll` + `bhaptics_library.lib` |
| macOS | Apple silicon + Intel (universal) | `sdk/lib/libbhaptics_library.dylib` |

## Contents

```
sdk/                      the SDK — copy this folder into your project
  include/bhaptics/bhaptics.h   C API (usable from C++)
  lib/                          native libraries
  cmake/                        find_package(bhaptics) support
  README.md                     setup without CMake (Visual Studio)
examples/                 runnable examples (below)
CMakeLists.txt            builds the examples
```

## Requirements

- **bHaptics Player** installed and running.
- A workspace on the [bHaptics Developer Portal](https://developer.bhaptics.com/) — it gives you the **SDK API key** and **workspace ID**, and holds the haptic events you design. See the [documentation portal](https://docs.bhaptics.com/portal/).
- Windows: the [Visual C++ 2015–2022 Redistributable](https://learn.microsoft.com/cpp/windows/latest-supported-vc-redist) matching your build (x64 or x86).
- CMake 3.21+ to build the examples.

## Run the examples

```bash
# Windows (-A Win32 for a 32-bit build)
cmake -S . -B build -A x64
cmake --build build --config Release
build\Release\hello.exe <sdk_api_key> <workspace_id> <event>

# macOS
cmake -S . -B build
cmake --build build
./build/hello <sdk_api_key> <workspace_id> <event>
```

Instead of arguments you can set `BHAPTICS_SDK_API_KEY`, `BHAPTICS_WORKSPACE_ID` and `BHAPTICS_EVENT`.

| Example | Shows |
|---|---|
| [`hello.c`](examples/hello.c) | The smallest complete program in plain C: connect, play one event, disconnect |
| [`play_events.cpp`](examples/play_events.cpp) | Shaping an event at runtime — strength, speed, rotation around the body, vertical offset, looping, stopping |
| [`motor_control.cpp`](examples/motor_control.cpp) | Driving TactSuit motors directly — a sweep and a heartbeat with `playDot`, a moving point with `playPath` |

Visual Studio 2019 or later opens this folder directly (**File → Open → Folder**) as a CMake project.

## Use the SDK in your project

With CMake:

```cmake
find_package(bhaptics CONFIG REQUIRED PATHS path/to/sdk)
target_link_libraries(my_app PRIVATE bhaptics::bhaptics)

# Windows: put bhaptics_library.dll next to the executable
add_custom_command(TARGET my_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:my_app> $<TARGET_FILE_DIR:my_app>
    COMMAND_EXPAND_LISTS)
```

The x64 or x86 library is picked to match your build. Without CMake, see [`sdk/README.md`](sdk/README.md).

```c
#include <bhaptics/bhaptics.h>

registryAndInit("YOUR_SDK_API_KEY", "YOUR_WORKSPACE_ID", "");
/* ...wait until wsIsConnected() — the connection is made in the background */
int32_t requestId = play("your_event");
/* ... */
wsClose();
bHapticsShutdown();
```

## API at a glance

Every function is declared and documented in [`bhaptics.h`](sdk/include/bhaptics/bhaptics.h).

| Area | Functions |
|---|---|
| Connection | `registryAndInit`, `registryAndInitHost`, `wsIsConnected`, `retryInitialize`, `wsClose`, `bHapticsShutdown` |
| Player | `isPlayerInstalled`, `isPlayerRunning`, `launchPlayer` |
| Events | `play`, `playParam`, `playWithStartTime`, `playWithoutResult`, `playLoop`, `getEventTime` |
| Playback control | `stop`, `stopByEventId`, `stopAll`, `pauseEvent`, `resume`, `isPlaying`, `isPlayingByEventId`, `isPlayingByRequestId` |
| Direct motor control | `playDot`, `playPath`, `playWaveform`, `playWaveformDk3` |
| Devices | `isbHapticsConnected`, `getDeviceInfoJson`, `ping`, `pingAll`, `swapPosition`, `setDeviceVsm` |
| Several devices of the same type | `playWithStartTimeToDevice`, `playLoopToDevice`, `playDotToDevice`, `playPathToDevice` |

Conventions:

- Calls are **synchronous**. Calls from several threads are serialized.
- `requestId` parameters: pass `-1` to let the SDK assign one (it is returned); any other value is used as-is, so you can later `stop()` it.
- `const char*` results are owned by the library and valid until the next call to the same function on the same thread — do not free them.
- Call `bHapticsShutdown()` before unloading the library (plugins, mods).

## Migrating from the previous version

The previous version shipped `tact-cpp2/tact-cpp2/library.h` and 64-bit libraries in `lib/`. Function signatures are unchanged except:

| Before | Now |
|---|---|
| `#include "library.h"` | `#include <bhaptics/bhaptics.h>` (add `sdk/include` to the include path) |
| `lib/bhaptics_library.{dll,lib}` | `sdk/lib/x64/` (and `sdk/lib/x86/` for 32-bit) |
| `pause(eventId)` | `pauseEvent(eventId)` |
| `reInitMessage(key, workspace, json)` | `retryInitialize(key, workspace)` |
| `bool resume(eventId)` | `void resume(eventId)` |
| `bHapticsGetHapticMappings/Message(…, int& status)` | `…, int32_t* status` — pass `&status` |

Rebuild your application after updating: replacing the DLL alone is not enough.

See [CHANGELOG.md](CHANGELOG.md) for everything new.

## License

Use of the bHaptics SDK is subject to the [bHaptics SDK agreement](https://bhaptics.gitbook.io/license-sdk/). See [LICENSE](LICENSE).
