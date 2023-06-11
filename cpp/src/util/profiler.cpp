#include "profiler.h"

#include <algorithm>
#include <functional>
#include <numeric>

#include "util.h"

profiler::profiler(int size)
{
    reserve(size);

    for (auto& elem : _values) {
        elem = 0;
    }
}

void profiler::add(long long val)
{
    _values.push_back(val);
}

void profiler::clear()
{
    _values.clear();
}

void profiler::reserve(int size)
{
    _values.reserve(size);
}

long long profiler::sum() const
{
    return std::accumulate(_values.begin(), _values.cend(), 0);
}

long long profiler::max() const
{
    assert(!_values.empty());
    return *std::max_element(_values.cbegin(), _values.cend());
}

long long profiler::min() const
{
    assert(!_values.empty());
    return *std::min_element(_values.cbegin(), _values.cend());
}

void profiler::print_profiler_stats()
{
    std::cout << "======= Stats =======" << std::endl;
    std::cout << "Minimum time: " << util::format_time(min()) << std::endl;
    std::cout << "Maximum time: " << util::format_time(max()) << std::endl;
}
