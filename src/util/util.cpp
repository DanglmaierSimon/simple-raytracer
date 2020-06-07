#include "util.h"

#include <random>

std::string format_time(std::chrono::nanoseconds time)
{
    using namespace std::chrono;

    auto s_str = std::to_string(duration_cast<seconds>(time).count());

    auto ms_str =
        left_pad_string(std::to_string(duration_cast<milliseconds>(time).count() % 1000), 3, '0');

    auto mcs_str =
        left_pad_string(std::to_string(duration_cast<microseconds>(time).count() % 1000), 3, '0');

    auto ns_str =
        left_pad_string(std::to_string(duration_cast<nanoseconds>(time).count() % 1000), 3, '0');

    return (s_str == "0" ? "" : s_str + "s ") + ms_str + "ms " + mcs_str + "µs " + ns_str + "ns";
}

std::string left_pad_string(std::string str, unsigned int length, char pad_char)
{
    if (length < str.size()) {
        return str;
    }

    return str.insert(0, length - str.size(), pad_char);
}

std::string format_time(long long time)
{
    return format_time(static_cast<std::chrono::nanoseconds>(time));
}

double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

double random_double()
{
    static std::random_device                     rand_dev;
    static std::mt19937                           generator(rand_dev());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}
