#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class profiler {
  public:
    profiler() = default;
    explicit profiler(int size)
    {
        reserve(size);

        for (auto& elem : _values) {
            elem = 0;
        }
    }

    void add(long long val)
    {
        _values.push_back(val);
    }

    void clear()
    {
        _values.clear();
    }

    void reserve(int size)
    {
        _values.reserve(size);
    }

    long long sum() const
    {
        return std::accumulate(_values.begin(), _values.cend(), 0);
    }

    long long max() const
    {
        assert(!_values.empty());
        return *std::max_element(_values.cbegin(), _values.cend());
    }

    long long min() const
    {
        assert(!_values.empty());
        return *std::min_element(_values.cbegin(), _values.cend());
    }

    void print_profiler_stats()
    {
        std::cout << "======= Stats =======" << std::endl;
        std::cout << "Minimum time: " << format_time(min()) << std::endl;
        std::cout << "Maximum time: " << format_time(max()) << std::endl;
    }

  private:
    std::vector<long long> _values;
};