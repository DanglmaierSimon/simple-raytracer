#pragma once

#include <chrono>

class elapsed_timer {
  public:
    elapsed_timer() = default;

    void start()
    {
        _start = std::chrono::steady_clock::now();
    }

    void restart()
    {
        _start = std::chrono::steady_clock::now();
    }

    int msecs_elapsed()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - _start)
            .count();
    }

  private:
    std::chrono::steady_clock::time_point _start;
};
