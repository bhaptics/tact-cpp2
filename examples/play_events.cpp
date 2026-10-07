// play_events.cpp - play an event from your workspace, then shape it at runtime:
// strength, speed, rotation around the body, vertical offset, looping and stopping.
//
//   play_events <sdk_api_key> <workspace_id> <event>
//   (or set BHAPTICS_SDK_API_KEY, BHAPTICS_WORKSPACE_ID, BHAPTICS_EVENT)
#include "example_common.hpp"

using namespace example;

namespace {

// Pass -1 as the request id to let the SDK assign one.
constexpr int32_t kAutoId = -1;

void step(const char* what) { std::printf("\n> %s\n", what); }

}  // namespace

int main(int argc, char** argv) {
    Args args;
    if (!parse_args(argc, argv, args, /*needs_event=*/true)) return 2;

    Session session(args);
    if (!session) return 1;

    const char* event = args.event;
    std::printf("Event \"%s\" lasts %d ms\n", event, getEventTime(event));

    step("As designed");
    wait_until_done(play(event));

    step("Half strength");
    wait_until_done(playParam(event, kAutoId, /*intensity=*/0.5f, /*duration=*/1.0f, 0.0f, 0.0f));

    step("Twice as long");
    wait_until_done(playParam(event, kAutoId, 1.0f, /*duration=*/2.0f, 0.0f, 0.0f));

    // angleX rotates the pattern around the body (0-360 degrees): 180 moves front to back.
    step("Rotated 180 degrees");
    wait_until_done(playParam(event, kAutoId, 1.0f, 1.0f, /*angleX=*/180.0f, 0.0f));

    // offsetY shifts it vertically (-0.5 up ... 0.5 down).
    step("Shifted down");
    wait_until_done(playParam(event, kAutoId, 1.0f, 1.0f, 0.0f, /*offsetY=*/0.3f));

    step("Three times, 300 ms apart");
    wait_until_done(playLoop(event, kAutoId, 1.0f, 1.0f, 0.0f, 0.0f, /*interval=*/300, /*maxCount=*/3));

    step("Looping until stopped after 2 s");
    const int32_t loop = playLoop(event, kAutoId, 1.0f, 1.0f, 0.0f, 0.0f, 300, 1000);
    sleep_for(2s);
    stop(loop);

    std::puts("\nDone.");
    return 0;
}
