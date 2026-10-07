// motor_control.cpp - drive TactSuit motors directly, without designed events.
//
//   playDot  - set every motor's intensity (0-100) for a duration
//   playPath - vibrate at arbitrary (x, y) points; the SDK picks and blends motors
//
//   motor_control <sdk_api_key> <workspace_id>
//   (or set BHAPTICS_SDK_API_KEY, BHAPTICS_WORKSPACE_ID)
#include <array>

#include "example_common.hpp"

using namespace example;

namespace {

// TactSuit motors as playDot sees them: 32 values, front then back, each side a
// 4 x 4 grid in row-major order (row 0 = shoulders, row 3 = waist).
constexpr int kCols = 4;
constexpr int kRows = 4;
using VestMotors = std::array<int32_t, 2 * kRows * kCols>;

constexpr int front(int row, int col) { return row * kCols + col; }

void play_motors(const VestMotors& motors, int32_t duration_ms) {
    playDot(kAutoId, Vest, duration_ms, motors.data(), static_cast<int32_t>(motors.size()));
}

// One row at a time, shoulders to waist.
void sweep_down_the_chest() {
    for (int row = 0; row < kRows; ++row) {
        VestMotors motors{};
        for (int col = 0; col < kCols; ++col) motors[front(row, col)] = 80;
        play_motors(motors, 150);
        sleep_for(150ms);
    }
}

// A heartbeat on one upper-chest motor: two quick pulses per beat.
void heartbeat(int beats) {
    VestMotors motors{};
    motors[front(1, 1)] = 100;
    for (int i = 0; i < beats; ++i) {
        play_motors(motors, 80);
        sleep_for(150ms);
        play_motors(motors, 80);
        sleep_for(650ms);
    }
}

// A single point travelling from one side of the chest to the other. Coordinates are
// 0.0-1.0 on each side of the vest; the SDK blends the neighbouring motors.
void swipe_across_the_chest() {
    constexpr int kSteps = 20;
    const float y = 0.3f;
    const int32_t intensity = 100;
    for (int i = 0; i <= kSteps; ++i) {
        const float x = static_cast<float>(i) / kSteps;
        playPath(kAutoId, Vest, 60, &x, &y, &intensity, 1);
        sleep_for(40ms);
    }
}

}  // namespace

int main(int argc, char** argv) {
    Args args;
    if (!parse_args(argc, argv, args, /*needs_event=*/false)) return 2;

    Session session(args);
    if (!session) return 1;

    if (!isbHapticsConnected(Vest)) {
        std::puts("No TactSuit connected - the calls below will have nothing to drive.");
    }

    std::puts("> Sweep down the chest (playDot)");
    sweep_down_the_chest();
    sleep_for(500ms);

    std::puts("> Heartbeat (playDot)");
    heartbeat(3);

    std::puts("> Swipe across the chest (playPath)");
    swipe_across_the_chest();

    stopAll();
    std::puts("\nDone.");
    return 0;
}
