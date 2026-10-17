#ifndef CELL_REPLAY_CLOCK_H
#define CELL_REPLAY_CLOCK_H
#include <algorithm>
#include <cstdint>

// Developer replays measure time at keyboard polls, excluding startup and
// limiting stalls so a 100 ms press/release pair cannot vanish between polls.
struct ReplayClock {
    uint64_t elapsed = 0, previous = 0;
    bool started = false;
    uint64_t poll(uint64_t now) {
        if (started) elapsed += std::min<uint64_t>(now - previous, 50000);
        previous = now;
        started = true;
        return elapsed;
    }
};
#endif
