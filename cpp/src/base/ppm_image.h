#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "vec3.h"

namespace colors {
using rgb = uint32_t;

constexpr uint red(rgb r)
{
    return (r >> 16) & 0xFF;
}

constexpr uint green(rgb r)
{
    return (r >> 8) & 0xFF;
}

constexpr uint blue(rgb r)
{
    return r & 0xFF;
}

constexpr rgb make_rgb(uint red, uint green, uint blue)
{
    return ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
}

constexpr rgb make_rgb(vec3 const& v, int const samples_per_pixel)
{
    // Divide the color total by the number of samples and gamma-correct
    // for a gamma value of 2.0.
    const auto scale = 1.0 / samples_per_pixel;

    const auto r = sqrt(scale * v.x());
    const auto g = sqrt(scale * v.y());
    const auto b = sqrt(scale * v.z());

    return make_rgb(static_cast<uint>(256 * std::clamp(r, 0.0, 0.999)),
                    static_cast<uint>(256 * std::clamp(g, 0.0, 0.999)),
                    static_cast<uint>(256 * std::clamp(b, 0.0, 0.999)));
}

} // namespace colors
class ppm_image {

  public:
    ppm_image(int width, int height, int samples_per_pixel, std::string file_name)
        : _width{width}
        , _height{height}
        , _samples_per_pixel{samples_per_pixel}
        , _file_name{file_name}
    {

        assert(width > 0);
        assert(height > 0);
        assert(samples_per_pixel > 0);

        _data.resize(_width * _height);
    }

    ~ppm_image() = default;

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    int size() const
    {
        return width() * height();
    }

    void set_color(int const x, int const y, colors::rgb const r)
    {
        int const index = y * width() + x;

        assert(index < size());

        _data[index] = r;
    }

    void set_color(int const x, int const y, vec3 const& v)
    {
        set_color(x, y, colors::make_rgb(v, _samples_per_pixel));
    }

    std::string filename() const
    {
        return _file_name;
    }

    void write_image()
    {
        using namespace colors;

        std::ofstream os(filename(), std::ios::trunc | std::ios::out);

        if (!os.is_open() || !os.good()) {
            std::cerr << "Error opening file!" << std::endl;
            os.close();

            return;
        }

        os << "P3\n" << width() << " " << height() << "\n255\n";

        // Traverse image in reverse order, otherwise its upside down
        for (auto it = _data.rbegin(); it != _data.rend(); it++) {
            const auto p = *it;
            os << std::to_string(red(p)) << " " << std::to_string(green(p)) << " "
               << std::to_string(blue(p)) << "\n";
        }

        os.close();
    }

  private:
    const int _width{0};
    const int _height{0};
    const int _samples_per_pixel{0};

    const std::string _file_name;

    std::vector<colors::rgb> _data;
};