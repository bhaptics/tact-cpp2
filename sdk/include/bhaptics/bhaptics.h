/*
 * bhaptics.h - bHaptics SDK C/C++ API
 *
 * Generated file. Do not edit.
 *
 * Strings: `const char*` return values are owned by the library and stay valid
 * until the next call to the same function on the same thread. Do not free them.
 *
 * Threading: every call blocks until it completes. Calls from several threads
 * are serialized on one internal worker thread; they never run in parallel.
 */

#ifndef BHAPTICS_H
#define BHAPTICS_H

#include <stdint.h>
#include <stdbool.h>

#define BHAPTICS_VERSION_MAJOR 2
#define BHAPTICS_VERSION_MINOR 7
#define BHAPTICS_VERSION_PATCH 1

/* Symbol import / visibility. Define it yourself (e.g. empty) to override.
   Not *_API: Unreal generates <MODULE>_API macros, so a module named
   "Bhaptics" would silently redefine it. */
#ifndef BHAPTICS_DECLSPEC
#  if defined(_WIN32) || defined(__CYGWIN__)
#    define BHAPTICS_DECLSPEC __declspec(dllimport)
#  elif defined(__GNUC__) && __GNUC__ >= 4
#    define BHAPTICS_DECLSPEC __attribute__((visibility("default")))
#  else
#    define BHAPTICS_DECLSPEC
#  endif
#endif

/* Calling convention, pinned so projects built with /Gz or /Gr still link. */
#ifndef BHAPTICS_CALL
#  if defined(_WIN32) && !defined(__GNUC__)
#    define BHAPTICS_CALL __cdecl
#  else
#    define BHAPTICS_CALL
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Register the app with the player.
 * @details Registers the app with the player, allowing connection to a workspace
 *          created on the developer site.
 * @param sdkAPIKey       The API key of the workspace.
 * @param workspaceId     The workspace ID of the app.
 * @param initJsonMessage Initial haptic message as a JSON-formatted string. Pass "" if unused.
 * @return `true` if the init pipeline started successfully. **This does NOT guarantee that the
 *         websocket is connected right now** - if the player is not running, a background
 *         auto-reconnect loop is armed and authentication is performed automatically once
 *         the player comes up. Use `wsIsConnected()` to check live connection state.
 *         `false` only when both the initial connect failed AND auto-reconnect is disabled,
 *         or when the synchronous auth message failed to send on a successful connect.
 *
 * # Safety
 * `sdkAPIKey`, `workspaceId`, and `initJsonMessage` must each be NULL or point
 * to a valid NUL-terminated C string that stays valid for the duration of this
 * call; NULL is treated as an empty string.
 */
BHAPTICS_DECLSPEC
bool BHAPTICS_CALL registryAndInit(const char *sdkAPIKey,
                     const char *workspaceId,
                     const char *initJsonMessage);

/**
 * @brief Register the app with an external player.
 * @details Like `registryAndInit`, but connects to a player running externally
 *          rather than locally on the same machine.
 * @param sdkAPIKey       The API key of the workspace.
 * @param workspaceId     The workspace ID of the app.
 * @param initJsonMessage Initial haptic message as a JSON-formatted string. Pass "" if unused.
 * @param url             Host (IP / hostname) of the external player.
 * @return Same semantics as `registryAndInit`: `true` means the init pipeline started
 *         successfully (connection may still be in progress via auto-reconnect), `false`
 *         means a fatal failure. Use `wsIsConnected()` to check live state.
 *
 * # Safety
 * `sdkAPIKey`, `workspaceId`, `initJsonMessage`, and `url` must each be NULL
 * or point to a valid NUL-terminated C string that stays valid for the
 * duration of this call; NULL is treated as an empty string.
 */
BHAPTICS_DECLSPEC
bool BHAPTICS_CALL registryAndInitHost(const char *sdkAPIKey,
                         const char *workspaceId,
                         const char *initJsonMessage,
                         const char *url);

/**
 * @brief Re-run the init handshake on the current connection.
 * @details Re-sends the registration/init payload over the existing connection
 *          to recover the authenticated session - e.g. after the player
 *          restarts or the workspace credentials are refreshed. Unlike
 *          `registryAndInit`, this does NOT open a new connection; call it only
 *          after an initial `registryAndInit`. Use `wsIsConnected()` to check
 *          live connection state.
 * @param sdkAPIKey   The API key of the workspace.
 * @param workspaceId The workspace ID of the app.
 *
 * # Safety
 * `sdkAPIKey` and `workspaceId` must each be NULL or point to a valid
 * NUL-terminated C string that stays valid for the duration of this call; NULL
 * is treated as an empty string.
 */
BHAPTICS_DECLSPEC void BHAPTICS_CALL retryInitialize(const char *sdkAPIKey, const char *workspaceId);

/**
 * @brief Check websocket connection status.
 * @return `true` if connected to the player, `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL wsIsConnected(void);

/**
 * @brief Close the websocket connection.
 */
BHAPTICS_DECLSPEC void BHAPTICS_CALL wsClose(void);

/**
 * @brief Shut down the internal runtime and release its background threads.
 * @details Closes the websocket connection, stops the auto-reconnect loop,
 * clears cached credentials, and then joins the internal worker thread so
 * no bHaptics code is still executing when a plugin host unloads this library
 * (`dlclose` / `FreeLibrary`). Call it once, on your teardown path, before the
 * library is unloaded - e.g. a Godot GDExtension deinitialize, a Unity
 * domain-reload / quit handler, or a UE module shutdown. Without it the worker
 * thread outlives the unload and dereferences unmapped code pages, crashing the
 * host.
 *
 * Distinct from `wsClose`, which only closes the connection and leaves the
 * runtime (and its worker thread) running; `wsClose` alone cannot prevent the
 * unload crash.
 *
 * Re-call contract - lazy re-creation: after shutdown the runtime is gone, and
 * the next call that needs it (e.g. `registryAndInit`) transparently builds a
 * fresh one. Calling `bHapticsShutdown` again while nothing is running is a
 * no-op. A stray call after shutdown therefore just re-initializes - there is
 * no use-after-shutdown crash.
 *
 * Thread-safety: do not call other exports concurrently with this on other
 * threads. A short grace period tolerates a call that is just returning, but
 * overlapping work defeats the point of joining.
 */
BHAPTICS_DECLSPEC void BHAPTICS_CALL bHapticsShutdown(void);

/**
 * @brief Play an event.
 * @details Plays an event defined in the workspace.
 * @param key         The defined event name.
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `key` must be NULL or point to a valid NUL-terminated C string that stays
 * valid for the duration of this call; NULL is treated as an empty string.
 */
BHAPTICS_DECLSPEC int32_t BHAPTICS_CALL play(const char *key);

/**
 * @brief Play an event with parameters.
 * @param key         The defined event name.
 * @param requestId   Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param intensity   Intensity multiplier (default 1.0).
 * @param duration    Duration multiplier (default 1.0).
 * @param angleX      X-angle offset (default 0.0).
 * @param offsetY     Y-angle offset (default 0.0).
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `key` must be NULL or point to a valid NUL-terminated C string that stays
 * valid for the duration of this call; NULL is treated as an empty string.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playParam(const char *key,
                  int32_t requestId,
                  float intensity,
                  float duration,
                  float angleX,
                  float offsetY);

/**
 * @brief Play an event without returning the request id.
 * @param key         The defined event name.
 * @param requestId   Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param intensity   Intensity multiplier (default 1.0).
 * @param duration    Duration multiplier (default 1.0).
 * @param angleX      X-angle offset (default 0.0).
 * @param offsetY     Y-angle offset (default 0.0).
 *
 * # Safety
 * `key` must be NULL or point to a valid NUL-terminated C string that stays
 * valid for the duration of this call; NULL is treated as an empty string.
 */
BHAPTICS_DECLSPEC
void BHAPTICS_CALL playWithoutResult(const char *key,
                       int32_t requestId,
                       float intensity,
                       float duration,
                       float angleX,
                       float offsetY);

/**
 * @brief Play an event starting from a specific offset.
 * @param key         The defined event name.
 * @param requestId   Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param startMillis Position in ms at which to start the event.
 * @param intensity   Intensity multiplier (default 1.0).
 * @param duration    Duration multiplier (default 1.0).
 * @param angleX      X-angle offset (default 0.0).
 * @param offsetY     Y-angle offset (default 0.0).
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `key` must be NULL or point to a valid NUL-terminated C string that stays
 * valid for the duration of this call; NULL is treated as an empty string.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playWithStartTime(const char *key,
                          int32_t requestId,
                          int32_t startMillis,
                          float intensity,
                          float duration,
                          float angleX,
                          float offsetY);

/**
 * @brief `playWithStartTime` on a single device, for setups with several
 *        devices of the same type connected (Windows/macOS). Also covers
 *        `play` / `playParam` (pass startMillis 0).
 * @param deviceIndex Index among the connected devices of the same type.
 *                    -1 plays on all of them, same as `playWithStartTime`.
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * Same as `playWithStartTime`.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playWithStartTimeToDevice(const char *key,
                                  int32_t requestId,
                                  int32_t startMillis,
                                  float intensity,
                                  float duration,
                                  float angleX,
                                  float offsetY,
                                  int32_t deviceIndex);

/**
 * @brief Run DotMode.
 * @param requestId     Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param position      The bHaptics product position to play on.
 * @param duration      Duration in ms.
 * @param motorValues   Pointer to an array of motor intensities (0-100).
 * @param motorValueLen Length of `motorValues` (typically up to 32).
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `motorValues` must be NULL or point to at least `motorValueLen` readable,
 * properly aligned `int32_t` values that stay valid for the duration of this
 * call. A NULL pointer or a non-positive `motorValueLen` is treated as an
 * empty array.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playDot(int32_t requestId,
                int32_t position,
                int32_t duration,
                const int32_t *motorValues,
                int32_t motorValueLen);

/**
 * @brief `playDot` on a single device, for setups with several devices of
 *        the same type connected (Windows/macOS).
 * @param deviceIndex Index among the connected devices of the same type.
 *                    -1 plays on all of them, same as `playDot`.
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * Same as `playDot`.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playDotToDevice(int32_t requestId,
                        int32_t position,
                        int32_t duration,
                        const int32_t *motorValues,
                        int32_t motorValueLen,
                        int32_t deviceIndex);

/**
 * @brief Run a waveform pattern (formerly `play_glove`).
 * @param requestId      Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param position       The bHaptics product position to play on.
 * @param motorValues    Per-motor intensity array (length = `motorLen`).
 * @param playTimeValues Per-motor play time (5ms units: 1=5ms, 2=10ms, 4=20ms, 6=30ms, 8=40ms).
 *                       Must have at least `motorLen` elements.
 * @param shapeValues    Per-motor shape (0: hold, 1: 50% linear decrease, 2: 50% linear increase).
 *                       Must have at least `motorLen` elements.
 * @param repeatCount    Number of repetitions.
 * @param motorLen       Length of all three arrays. **Caller must guarantee that
 *                       `motorValues`, `playTimeValues`, and `shapeValues` are each
 *                       at least this long; otherwise the read is out-of-bounds.**
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `motorValues`, `playTimeValues`, and `shapeValues` must each be NULL or
 * point to at least `motorLen` readable, properly aligned `int32_t` values
 * that stay valid for the duration of this call. A NULL pointer or a
 * non-positive `motorLen` is treated as an empty array; a `motorLen` larger
 * than any of the three allocations is an out-of-bounds read.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playWaveform(int32_t requestId,
                     int32_t position,
                     const int32_t *motorValues,
                     const int32_t *playTimeValues,
                     const int32_t *shapeValues,
                     int32_t repeatCount,
                     int32_t motorLen);

/**
 * @brief Run a waveform pattern on the TactGlove DK3 variant.
 * @param requestId      Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param position       The bHaptics product position to play on.
 * @param motorValues    Per-motor intensity array (length = `motorLen`).
 * @param playTimeValues Per-motor play time (5ms units: 1=5ms, 2=10ms, 4=20ms, 6=30ms, 8=40ms).
 *                       Must have at least `motorLen` elements.
 * @param shapeValues    Per-motor shape (0: hold, 1: 50% linear decrease, 2: 50% linear increase).
 *                       Must have at least `motorLen` elements.
 * @param frequency      Frequency value forwarded to the device.
 * @param repeatCount    Number of repetitions.
 * @param motorLen       Length of all three arrays. **Caller must guarantee that
 *                       `motorValues`, `playTimeValues`, and `shapeValues` are each
 *                       at least this long; otherwise the read is out-of-bounds.**
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `motorValues`, `playTimeValues`, and `shapeValues` must each be NULL or
 * point to at least `motorLen` readable, properly aligned `int32_t` values
 * that stay valid for the duration of this call. A NULL pointer or a
 * non-positive `motorLen` is treated as an empty array; a `motorLen` larger
 * than any of the three allocations is an out-of-bounds read.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playWaveformDk3(int32_t requestId,
                        int32_t position,
                        const int32_t *motorValues,
                        const int32_t *playTimeValues,
                        const int32_t *shapeValues,
                        int32_t frequency,
                        int32_t repeatCount,
                        int32_t motorLen);

/**
 * @brief Run a path pattern.
 * @param requestId       Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param position        The bHaptics product position to play on.
 * @param durationMillis  Duration in ms.
 * @param xValues         Array of x coordinates (length = `len`).
 * @param yValues         Array of y coordinates (length = `len`).
 * @param intensityValues Array of intensities (length = `len`).
 * @param len             Length of the three coordinate arrays.
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * `xValues` and `yValues` must each be NULL or point to at least `len`
 * readable, properly aligned `float` values, and `intensityValues` NULL or at
 * least `len` readable, properly aligned `int32_t` values; all must stay valid
 * for the duration of this call. A NULL pointer or a non-positive `len` is
 * treated as an empty array; a `len` larger than any of the three allocations
 * is an out-of-bounds read.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playPath(int32_t requestId,
                 int32_t position,
                 int32_t durationMillis,
                 const float *xValues,
                 const float *yValues,
                 const int32_t *intensityValues,
                 int32_t len);

/**
 * @brief `playPath` on a single device, for setups with several devices of
 *        the same type connected (Windows/macOS).
 * @param deviceIndex Index among the connected devices of the same type.
 *                    -1 plays on all of them, same as `playPath`.
 * @return The request id assigned to the playback. -1 if playback failed to start.
 *
 * # Safety
 * Same as `playPath`.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playPathToDevice(int32_t requestId,
                         int32_t position,
                         int32_t durationMillis,
                         const float *xValues,
                         const float *yValues,
                         const int32_t *intensityValues,
                         int32_t len,
                         int32_t deviceIndex);

/**
 * @brief Loop an event.
 * @details Plays an event repeatedly with an interval between repetitions.
 * @param eventId     The defined event name.
 * @param requestId   Request id to assign, used verbatim. Pass -1 to let the SDK generate one.
 * @param intensity   Intensity multiplier (default 1.0).
 * @param duration    Duration multiplier (default 1.0).
 * @param angleX      X-angle offset (default 0.0).
 * @param offsetY     Y-angle offset (default 0.0).
 * @param interval    Interval (ms) between repetitions.
 * @param maxCount    Number of repetitions.
 * @return The request id assigned to the loop. -1 if playback failed to start.
 *
 * # Safety
 * `eventId` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playLoop(const char *eventId,
                 int32_t requestId,
                 float intensity,
                 float duration,
                 float angleX,
                 float offsetY,
                 int32_t interval,
                 int32_t maxCount);

/**
 * @brief `playLoop` on a single device, for setups with several devices of
 *        the same type connected (Windows/macOS).
 * @param deviceIndex Index among the connected devices of the same type.
 *                    -1 plays on all of them, same as `playLoop`.
 * @return The request id assigned to the loop. -1 if playback failed to start.
 *
 * # Safety
 * Same as `playLoop`.
 */
BHAPTICS_DECLSPEC
int32_t BHAPTICS_CALL playLoopToDevice(const char *eventId,
                         int32_t requestId,
                         float intensity,
                         float duration,
                         float angleX,
                         float offsetY,
                         int32_t interval,
                         int32_t maxCount,
                         int32_t deviceIndex);

/**
 * @brief Get the play duration of an event.
 * @param eventId The defined event name.
 * @return Event duration in ms.
 *
 * # Safety
 * `eventId` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC int32_t BHAPTICS_CALL getEventTime(const char *eventId);

/**
 * @brief Pause a running event.
 * @param eventId The id of the event to pause.
 *
 * # Safety
 * `eventId` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC void BHAPTICS_CALL pauseEvent(const char *eventId);

/**
 * @brief Resume a paused event.
 * @param eventId The id of the event to resume.
 *
 * # Safety
 * `eventId` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC void BHAPTICS_CALL resume(const char *eventId);

/**
 * @brief Stop an event by request id.
 * @param requestKey The request id returned at play time.
 * @return `true` if the stop request was queued for transmission, `false` on
 *         encode/transport failure (e.g. the Player is not connected). This
 *         reports delivery of the request, not that the device has stopped.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL stop(int32_t requestKey);

/**
 * @brief Stop an event by event id.
 * @param eventId The event id to stop.
 * @return `true` if the stop request was queued for transmission, `false` on
 *         encode/transport failure. Reports delivery of the request, not that
 *         the device has stopped.
 *
 * # Safety
 * `eventId` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL stopByEventId(const char *eventId);

/**
 * @brief Stop all running events.
 * @return `true` if the stop-all request was queued for transmission, `false`
 *         on encode/transport failure (e.g. the Player is not connected). This
 *         reports delivery of the request, not that the device has stopped.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL stopAll(void);

/**
 * @brief Check the connection status of a specific device.
 * @param position Device product position.
 * @return `true` if connected, `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL isbHapticsConnected(int32_t position);

/**
 * @brief Check whether any event is currently playing.
 * @return `true` if at least one event is running, `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL isPlaying(void);

/**
 * @brief Check whether an event is playing by request id.
 * @param requestId The request id returned at play time.
 * @return `true` if the event is running, `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL isPlayingByRequestId(int32_t requestId);

/**
 * @brief Check whether an event is playing by event id.
 * @param eventId Event name.
 * @return `true` if the event is running, `false` otherwise.
 *
 * # Safety
 * `eventId` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL isPlayingByEventId(const char *eventId);

/**
 * @brief Fetch the haptic message bundle for the app from the bHaptics server.
 * @param appKey      App API key.
 * @param workspaceId Workspace id.
 * @param lastVersion Last known version, or -1 to fetch the latest.
 * @param status      Out: `BhapticsStatusCode`-mapped value from the HTTP response
 *                    (0=Success, 100=NotChanged, 2=ApiKeyError, 3=WorkspaceError,
 *                    4=WorkspaceDeployError, 999=UnknownError). **Always written
 *                    before this function returns**: set to 0 on entry, overwritten
 *                    with the real value if the HTTP call completes, and left at 0
 *                    if the call times out or fails. May be `NULL`.
 * @return JSON string owned by the library. The pointer is valid until the next
 *         call to this function from the same thread. Do NOT `free()` it.
 *         Returns `NULL` if an unrecoverable error (e.g. allocator failure) occurs.
 *
 * # Safety
 * `appKey` and `workspaceId` must each be NULL or point to a valid
 * NUL-terminated C string that stays valid for the duration of this call; NULL
 * is treated as an empty string. `status` must be NULL or point to a writable,
 * properly aligned `int32_t` - it is written before this function returns.
 *
 * The returned pointer is owned by the library and is invalidated by the next
 * call to this function on the same thread - do not `free()` it and do not use
 * it from another thread.
 */
BHAPTICS_DECLSPEC
const char *BHAPTICS_CALL bHapticsGetHapticMessage(const char *appKey,
                                     const char *workspaceId,
                                     int32_t lastVersion,
                                     int32_t *status);

/**
 * @brief Fetch the haptic mapping bundle for the app from the bHaptics server.
 * @param appKey      App API key.
 * @param workspaceId Workspace id.
 * @param lastVersion Last known version, or -1 to fetch the latest.
 * @param status      Out: `BhapticsStatusCode`-mapped value from the HTTP response
 *                    (0=Success, 100=NotChanged, 2=ApiKeyError, 3=WorkspaceError,
 *                    4=WorkspaceDeployError, 999=UnknownError). **Always written
 *                    before this function returns**: set to 0 on entry, overwritten
 *                    with the real value if the HTTP call completes, and left at 0
 *                    if the call times out or fails. May be `NULL`.
 * @return JSON string owned by the library. The pointer is valid until the next
 *         call to this function from the same thread. Do NOT `free()` it.
 *         Returns `NULL` if an unrecoverable error (e.g. allocator failure) occurs.
 *
 * # Safety
 * `appKey` and `workspaceId` must each be NULL or point to a valid
 * NUL-terminated C string that stays valid for the duration of this call; NULL
 * is treated as an empty string. `status` must be NULL or point to a writable,
 * properly aligned `int32_t` - it is written before this function returns.
 *
 * The returned pointer is owned by the library and is invalidated by the next
 * call to this function on the same thread - do not `free()` it and do not use
 * it from another thread.
 */
BHAPTICS_DECLSPEC
const char *BHAPTICS_CALL bHapticsGetHapticMappings(const char *appKey,
                                      const char *workspaceId,
                                      int32_t lastVersion,
                                      int32_t *status);

/**
 * @brief Check whether the bHaptics Player process is running.
 * @return `true` if the player is running, `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL isPlayerRunning(void);

/**
 * @brief Check whether the bHaptics Player is installed on this machine.
 * @return `true` if the player is installed, `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL isPlayerInstalled(void);

/**
 * @brief Launch the bHaptics Player.
 * @param tryLaunch Whether to attempt launching if not already running.
 * @return `true` if the player is launched (or already running), `false` otherwise.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL launchPlayer(bool tryLaunch);

/**
 * @brief Get information about connected devices.
 * @return JSON-formatted string describing connected devices. The pointer is owned
 *         by the library and is valid until the next call to this function from
 *         the same thread. Do NOT `free()` it.
 *         Returns `NULL` if an unrecoverable error (e.g. allocator failure) occurs.
 *
 * The returned pointer is owned by the library and is invalidated by the next
 * call to this function on the same thread - do not `free()` it and do not use
 * it from another thread.
 */
BHAPTICS_DECLSPEC const char *BHAPTICS_CALL getDeviceInfoJson(void);

/**
 * @brief Get the haptic event list for this app.
 * @return JSON-formatted string of the haptic events. The pointer is owned by
 *         the library and is valid until the next call to this function from
 *         the same thread. Do NOT `free()` it.
 *         Returns `NULL` if an unrecoverable error (e.g. allocator failure) occurs.
 *
 * The returned pointer is owned by the library and is invalidated by the next
 * call to this function on the same thread - do not `free()` it and do not use
 * it from another thread.
 */
BHAPTICS_DECLSPEC const char *BHAPTICS_CALL getHapticMappingsJson(void);

/**
 * @brief Ping a specific device.
 * @details Sends a ping to a specific device connected to the player.
 * @param address The MAC address of the device.
 * @return `true` if the message was sent, `false` if the player is not connected.
 *
 * # Safety
 * `address` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL ping(const char *address);

/**
 * @brief Ping all connected devices.
 * @return `true` if the message was sent, `false` if the player is not connected.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL pingAll(void);

/**
 * @brief Swap the left/right side of a device.
 * @details Function for devices that support left/right swapping.
 * @param address The MAC address of the device.
 * @return `true` if the swap was sent, `false` if the player is not connected.
 *
 * # Safety
 * `address` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL swapPosition(const char *address);

/**
 * @brief Change the VSM value of a device.
 * @param address The MAC address of the device.
 * @param vsm     The VSM value to set (0-400).
 * @return `true` if the change was sent, `false` if the player is not connected.
 *
 * # Safety
 * `address` must be NULL or point to a valid NUL-terminated C string that
 * stays valid for the duration of this call; NULL is treated as an empty
 * string.
 */
BHAPTICS_DECLSPEC bool BHAPTICS_CALL setDeviceVsm(const char *address, int32_t vsm);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  /* BHAPTICS_H */
