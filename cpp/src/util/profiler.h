#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class profiler {
  public:
    profiler() = default;
    explicit profiler(int size);

    void add(long long val);

    void clear();

    void reserve(int size);

    long long sum() const;
    long long max() const;
    long long min() const;

    void print_profiler_stats();

  private:
    std::vector<long long> _values;
};
