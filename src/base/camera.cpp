#include "camera.h"

ray camera::get_ray(double s, double t)
{
    vec3 rd     = _lens_radius * random_in_unit_disk();
    vec3 offset = _u * rd.x() + _v * rd.y();

    return ray(_origin + offset,
               _lower_left_corner + s * _horicontal + t * _vertical - _origin - offset);
}
