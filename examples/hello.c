/*
 * hello.c - the smallest complete program: connect, play one event, disconnect.
 *
 *   hello <sdk_api_key> <workspace_id> <event>
 *   (or set BHAPTICS_SDK_API_KEY, BHAPTICS_WORKSPACE_ID, BHAPTICS_EVENT)
 */
#include <stdio.h>
#include <stdlib.h>

#include <bhaptics/bhaptics.h>

#ifdef _WIN32
#include <windows.h>
static void sleep_ms(int ms) { Sleep((DWORD)ms); }
#else
#include <time.h>
static void sleep_ms(int ms) {
    struct timespec t = {ms / 1000, (long)(ms % 1000) * 1000000L};
    nanosleep(&t, NULL);
}
#endif

int main(int argc, char **argv) {
    const char *api_key      = argc > 1 ? argv[1] : getenv("BHAPTICS_SDK_API_KEY");
    const char *workspace_id = argc > 2 ? argv[2] : getenv("BHAPTICS_WORKSPACE_ID");
    const char *event        = argc > 3 ? argv[3] : getenv("BHAPTICS_EVENT");
    if (!api_key || !workspace_id || !event) {
        fprintf(stderr, "usage: %s <sdk_api_key> <workspace_id> <event>\n", argv[0]);
        return 2;
    }

    if (!registryAndInit(api_key, workspace_id, "")) {
        fprintf(stderr, "registryAndInit failed\n");
        return 1;
    }

    /* The Player connects, then sends the workspace's event list, in the
       background: wait up to 10 s. play() does not check event names. */
    for (int i = 0; i < 100 && getEventTime(event) <= 0; i++) {
        sleep_ms(100);
    }

    int status = 1;
    if (!wsIsConnected()) {
        fprintf(stderr, "Could not reach the bHaptics Player. Is it running?\n");
    } else if (getEventTime(event) <= 0) {
        fprintf(stderr, "No event \"%s\" in this workspace. Is it deployed?\n", event);
    } else {
        int32_t request_id = play(event);
        printf("Playing \"%s\" (request %d)\n", event, request_id);

        sleep_ms(100); /* playback starts asynchronously; wait up to 10 s for it to end */
        for (int i = 0; i < 500 && isPlayingByRequestId(request_id); i++) {
            sleep_ms(20);
        }
        status = 0;
    }

    wsClose();
    bHapticsShutdown();
    return status;
}
