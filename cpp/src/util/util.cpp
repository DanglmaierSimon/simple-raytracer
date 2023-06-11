#include "util.h"

#include <random>

std::string util::format_time(std::chrono::nanoseconds time)
{
    using namespace std::chrono;

    const auto s_str = std::to_string(duration_cast<seconds>(time).count());

    const auto ms_str =
        util::left_pad_string(std::to_string(duration_cast<milliseconds>(time).count() % 1000),
                              3,
                              '0');

    const auto mcs_str =
        util::left_pad_string(std::to_string(duration_cast<microseconds>(time).count() % 1000),
                              3,
                              '0');

    const auto ns_str =
        util::left_pad_string(std::to_string(duration_cast<nanoseconds>(time).count() % 1000),
                              3,
                              '0');

    return (s_str == "0" ? "" : s_str + "s ") + ms_str + "ms " + mcs_str + "µs " + ns_str + "ns";
}

std::string util::left_pad_string(std::string str, unsigned int length, char pad_char)
{
    if (length < str.size()) {
        return str;
    }

    return str.insert(0, length - str.size(), pad_char);
}

std::string util::format_time(long long time)
{
    return format_time(static_cast<std::chrono::nanoseconds>(time));
}

double util::random_double(double min, double max)
{
    return min + (max - min) * util::random_double();
}

double util::random_double()
{
#ifdef USE_FIXED_RANDOM
    // Use fixed randomness for benchmarks
    static std::mt19937 generator(0);
#else
    static std::random_device rand_dev;
    static std::mt19937       generator(rand_dev());
#endif

    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}