# bHaptics SDK for C / C++

Native library and header for driving bHaptics devices (TactSuit, TactGlove, TactVisor, TactSleeve, …) from C and C++ through the bHaptics Player.

## Contents

```
include/bhaptics/bhaptics.h      C API (also usable from C++)
lib/                             native library for this platform
  x64/ x86/                        Windows: bhaptics_library.dll + bhaptics_library.lib (import library)
  libbhaptics_library.so           Linux
  libbhaptics_library.dylib        macOS (universal: x86_64 + arm64)
cmake/                           find_package(bhaptics) support
THIRD_PARTY_NOTICES.txt          licenses of the open-source components inside the library
```

## Requirements

- [bHaptics Player](https://www.bhaptics.com/software/player/?type=pcplayer) running on the machine (or reachable — see `registryAndInitHost`)
- A workspace on the [bHaptics Developer Portal](https://developer.bhaptics.com/) for your SDK API key and workspace ID
- Windows 10 or later with the [Visual C++ 2015–2022 Redistributable](https://learn.microsoft.com/cpp/windows/latest-supported-vc-redist) matching your build (x64 or **x86**)
- Any C99 / C++11 compiler. On Windows the calling convention is pinned to `__cdecl`, so `/Gz` and `/Gr` projects work too

## Use with CMake

```cmake
find_package(bhaptics CONFIG REQUIRED)     # -Dbhaptics_DIR=<this folder>/cmake
target_link_libraries(my_app PRIVATE bhaptics::bhaptics)

# Windows: put the DLL next to the executable
add_custom_command(TARGET my_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:my_app> $<TARGET_FILE_DIR:my_app>
    COMMAND_EXPAND_LISTS)
```

On Windows the x64 or x86 library is selected from the project's architecture (`-A x64` / `-A Win32`).

## Use with Visual Studio (without CMake)

1. **C/C++ → General → Additional Include Directories**: add `include`
2. **Linker → General → Additional Library Directories**: add `lib\x64` (or `lib\x86` for Win32)
3. **Linker → Input → Additional Dependencies**: add `bhaptics_library.lib`
4. Copy `lib\<arch>\bhaptics_library.dll` next to your executable

## Quick start

```c
#include <bhaptics/bhaptics.h>

int main(void) {
    if (!registryAndInit("YOUR_SDK_API_KEY", "YOUR_WORKSPACE_ID", "")) {
        return 1;
    }
    /* The Player connection is made in the background — poll wsIsConnected()
       before the first play if you need it to land immediately. */

    int32_t requestId = play("your_event");   /* an event from your workspace */

    /* ... */

    stop(requestId);
    wsClose();
    bHapticsShutdown();                        /* before unloading the library */
    return 0;
}
```

Every call is synchronous. `const char*` results are owned by the library — do not free them.

## API key and workspace ID

`registryAndInit` takes the SDK API key and workspace ID of your Developer Portal workspace. They identify your workspace to the bHaptics Player and are **not secrets**: your application has to contain them and sends them on every connection, so anyone with your build can read them. Hiding or obfuscating them does not help.

- Embedding them in your application, or in a config file shipped with it, is fine.
- Use one workspace per application, not one shared across unrelated projects.
- If your source code is public, commit placeholders and fill in the real values in your release build. People who fork your project should create their own workspace instead of reusing yours.

## Redistribution

The runtime libraries (`bhaptics_library.dll`, `libbhaptics_library.so`, `libbhaptics_library.dylib`), the import library `bhaptics_library.lib` and `include/bhaptics/bhaptics.h` are bHaptics Redistributables under the [bHaptics SDK agreement](https://bhaptics.gitbook.io/license-sdk/): you may ship them with your application, free or commercial, and keep them in your project's source repository. At run time your application only needs the runtime library for its platform.

When you distribute them:

- Include `THIRD_PARTY_NOTICES.txt`. The library contains open-source components whose licenses require these notices to travel with it.
- Ship the files unmodified.
- Do not bundle the bHaptics Player. Point users to the [download page](https://www.bhaptics.com/software/player/?type=pcplayer).
- The files stay under the bHaptics SDK agreement, not your project's license. If your project is open source, say so next to them, for example: "bhaptics_library and bhaptics.h are © bHaptics Inc. and distributed under the bHaptics SDK agreement."
- Do not suggest that bHaptics partners with, sponsors or endorses your project.

## Examples and documentation

- Examples: <https://github.com/bhaptics/tact-cpp2>
- Documentation: <https://docs.bhaptics.com/>

## License

Use of the bHaptics SDK is subject to the [bHaptics SDK agreement](https://bhaptics.gitbook.io/license-sdk/). The open-source components inside the library are listed in `THIRD_PARTY_NOTICES.txt`.
