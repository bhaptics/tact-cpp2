// Shared setup for the C++ examples: credentials, Player connection, waiting.
#pragma once

#include <bhaptics/bhaptics.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace example {

using namespace std::chrono_literals;

// Device positions accepted by playDot / playPath / isbHapticsConnected.
enum Position : int32_t {
    Vest = 0,  // TactSuit
    ForearmL, ForearmR,
    Head,      // TactVisor
    HandL, HandR,
    FootL, FootR,
    GloveL, GloveR,
};

// Pass as a requestId to let the SDK assign one.
constexpr int32_t kAutoId = -1;

inline void sleep_for(std::chrono::milliseconds d) { std::this_thread::sleep_for(d); }

// Credentials come from the command line or the environment, never from source code.
struct Args {
    const char* api_key = nullptr;
    const char* workspace_id = nullptr;
    const char* event = nullptr;  // only for examples that play a workspace event
};

inline bool parse_args(int argc, char** argv, Args& out, bool needs_event) {
    out.api_key      = argc > 1 ? argv[1] : std::getenv("BHAPTICS_SDK_API_KEY");
    out.workspace_id = argc > 2 ? argv[2] : std::getenv("BHAPTICS_WORKSPACE_ID");
    out.event        = argc > 3 ? argv[3] : std::getenv("BHAPTICS_EVENT");
    if (out.api_key && out.workspace_id && (out.event || !needs_event)) {
        return true;
    }
    std::fprintf(stderr,
                 "usage: %s <sdk_api_key> <workspace_id>%s\n"
                 "   or set BHAPTICS_SDK_API_KEY, BHAPTICS_WORKSPACE_ID%s\n"
                 "Both come from your workspace on https://developer.bhaptics.com\n",
                 argv[0], needs_event ? " <event>" : "", needs_event ? ", BHAPTICS_EVENT" : "");
    return false;
}

// Registers with the bHaptics Player on construction and disconnects on scope exit.
// Ready once connected and, if args.event is set, once the Player knows that event.
class Session {
public:
    explicit Session(const Args& args, std::chrono::seconds timeout = 10s) {
        if (!isPlayerRunning()) {
            std::puts("Starting bHaptics Player...");
            launchPlayer(true);
        }
        if (!registryAndInit(args.api_key, args.workspace_id, "")) {
            std::fputs("registryAndInit failed\n", stderr);
            return;
        }
        // The connection is made in the background; wait for it before playing.
        const auto deadline = std::chrono::steady_clock::now() + timeout;
        while (!wsIsConnected() && std::chrono::steady_clock::now() < deadline) {
            sleep_for(100ms);
        }
        if (!wsIsConnected()) {
            std::fputs("Could not reach the bHaptics Player. Is it installed and running?\n", stderr);
            return;
        }
        // The Player then sends the workspace's event list. play() does not check event
        // names, so a typo would otherwise play nothing, silently.
        while (args.event && getEventTime(args.event) <= 0 && std::chrono::steady_clock::now() < deadline) {
            sleep_for(100ms);
        }
        if (args.event && getEventTime(args.event) <= 0) {
            std::fprintf(stderr, "No event \"%s\" in this workspace. Is it deployed?\n", args.event);
            return;
        }
        ready_ = true;
    }

    ~Session() {
        wsClose();
        bHapticsShutdown();
    }

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    explicit operator bool() const { return ready_; }

private:
    bool ready_ = false;
};

// Blocks until the playback with this request id has finished, or the timeout passes.
inline void wait_until_done(int32_t request_id, std::chrono::milliseconds timeout = 10s) {
    sleep_for(100ms);  // playback starts asynchronously in the Player
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (isPlayingByRequestId(request_id) && std::chrono::steady_clock::now() < deadline) {
        sleep_for(20ms);
    }
}

}  // namespace example
