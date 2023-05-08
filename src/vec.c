#include "vec.h"
#include <math.h>

float vec2_len(t_vec2 v)
{
    int len2;

    len2 = v.x * v.x + v.y * v.y;
    if (len2 != 0)
	return (sqrtf(len2));
    return (len2);
}

float vec3f_len(t_vec3f v)
{
    float len2;

    len2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (len2 != 0.0f)
        return (sqrtf(len2));
    return (len2);
}

t_vec3f vec3f_norm(t_vec3f v)
{
    float len;

    len = vec3f_len(v);
    if (len != 0.0f)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return (v);
}

t_vec3f vec3f_scale(t_vec3f v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return (v);
}
