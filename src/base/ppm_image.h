#include <cassert>

namespace colors {
using rgb = int;

constexpr int red(rgb r)
{
    return (r >> 16) & 0xFF;
}

constexpr int green(rgb r)
{
    return (r >> 8) & 0xFF;
}

constexpr int blue(rgb r)
{
    return r & 0xFF;
}

constexpr rgb makeRgb(int red, int green, int blue)
{
    return ((red & 0xFF) << 16) & ((green & 0xFF) << 8) & (blue & 0xFF);
}

} // namespace colors
class ppm_image {

  public:
    ppm_image(int width, int height)
        : _width{width}
        , _height{height}
    {
        if (size() != 0) {
            _data = new colors::rgb[size()];
        }
    }

    ~ppm_image()
    {
        delete[] _data;
        _data = nullptr;
    }

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

    bool setColor(int x, int y, colors::rgb r)
    {
        int index = y * width() + x;

        assert(index < size());

        if (index >= size()) {
            return false;
        }

        _data[index] = r;
    }

  private:
    int _width{0};
    int _height{0};

    colors::rgb* _data{nullptr};
};