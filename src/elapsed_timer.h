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
        using namespace std::chrono;
        return duration_cast<milliseconds>(steady_clock::now() - _start).count();
    }

    int nsecs_elapsed()
    {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(steady_clock::now() - _start).count();
    }

    std::chrono::nanoseconds elapsed()
    {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(steady_clock::now() - _start);
    }

  private:
    std::chrono::steady_clock::time_point _start;
};
