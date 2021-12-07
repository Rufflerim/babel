#include "Timer.h"
#include <SDL_timer.h>

using engine::Timer;

GameTime Timer::computeTime(const GameTime& time)
{
    frameStartTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<f64> frameDelay = frameStartTime - frameEndTime;

    f64 dt = frameDelay.count();
    GameTime computedTime { dt, time.totalTime + dt };

    frameEndTime = frameStartTime;

    return computedTime;
}

void Timer::delayTime()
{
    auto frameEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration<f64> frameDelayWithoutMinDuration = frameEnd - frameStartTime;
    f64 frameDelayWithoutMinDurationSeconds = frameDelayWithoutMinDuration.count();
    if (frameDelayWithoutMinDurationSeconds < MIN_FRAME_DURATION) {
        u32 sleepTime = static_cast<u32>((MIN_FRAME_DURATION - frameDelayWithoutMinDurationSeconds) * 1000);
        SDL_Delay(sleepTime);
    }
}
