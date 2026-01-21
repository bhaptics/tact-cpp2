# Tact-CPP2 Project

A C++ library for integrating bHaptics haptic feedback devices.

> **Platform**: Windows only (Windows 10/11)

## Requirements

- Windows 10 or Windows 11
- Visual Studio 2019 or later
- [bHaptics Player](https://www.bhaptics.com/software/player/?type=pcplayer) (must be running)

## Getting Started

1. Create a workspace at the [bHaptics Developer Portal](https://developer.bhaptics.com/).
   - For documentation, refer to the [bHaptics Documentation Portal](https://docs.bhaptics.com/portal/).

2. Download and install the **bHaptics Player** from [here](https://www.bhaptics.com/software/player/?type=pcplayer).
   - The player must be running for the library to work.

## Project Structure

```
tact-cpp2/
├── lib/
│   ├── bhaptics_library.dll
│   └── bhaptics_library.lib
└── tact-cpp2/
    ├── tact-cpp2.sln
    └── tact-cpp2/
        ├── library.h          # API header
        └── main.cpp           # Usage example
```

## Build

1. Open `tact-cpp2/tact-cpp2.sln` in Visual Studio.
2. Build the solution (F7 or Build > Build Solution).

## Usage

To use this library in your project, you need the following files:

- `bhaptics_library.dll` (Runtime DLL, from the `lib/` directory)
- `bhaptics_library.lib` (Link library, from the `lib/` directory)
- `library.h` (Header file located at `tact-cpp2/tact-cpp2/library.h`)

### Example

```cpp
#include "library.h"

int main() {
    // TODO: Replace these placeholder values with your own from the bHaptics Developer Portal.
    const char* sdkKey = "YOUR_SDK_KEY_HERE";   // Obtain from your workspace at https://developer.bhaptics.com
    const char* appId = "YOUR_APP_ID_HERE";     // App ID from the same workspace
    const char* mappingJson = "{}";             // JSON string with your haptic mappings
    // Initialize connection
    bool connected = registryAndInit(sdkKey, appId, mappingJson);

    if (!connected) {
        return -1;
    }

    // Play haptic event
    // "eventName" must match the name of an event defined in your bHaptics Developer Portal workspace (or in your mapping JSON).
    int requestId = play("eventName");

    // Check if playing
    bool playing = isPlayingByRequestId(requestId);

    // Stop event
    stop(requestId);

    // Close connection
    wsClose();

    return 0;
}
```

For more detailed usage, refer to `tact-cpp2/tact-cpp2/main.cpp`.

## License

See [LICENSE](LICENSE) file. Use of bHaptics SDK is subject to the [bHaptics SDK agreement](https://bhaptics.gitbook.io/license-sdk/).
