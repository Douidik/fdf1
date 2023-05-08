#include "camera.h"
#include "cohen_sutherland.h"
#include "map.h"
#include "render.h"
#include "util.h"
#include "window.h"
#include <libft.h>
#include <math.h>
#include <mlx.h>

#include <assert.h>

#define FDF_COLOR_MIN (0x114357)
#define FDF_COLOR_MAX (0xF29492)

/* #define FDF_COLOR_MIN (0xFFFFFF) */
/* #define FDF_COLOR_MAX (0xF29492) */

int fdf_vertex_make(t_fdf_renderer *render, t_fdf_camera *cam, t_mat4 *mvp, t_vec2 pos)
{
    size_t i;
    t_fdf_vertex *v;
    t_vec4f p;

    i = pos.y * render->map->w + pos.x;
    assert(i < render->map->w * render->map->h);
    v = &render->vs[i];

    if (pos.x < 0 || pos.x >= render->map->w || pos.y < 0 || pos.y >= render->map->h)
        return (v->ok = 0);
    v->pos = pos;
    v->height = render->map->buf[i] * cam->height;
    v->color = fdf_lerp_rgb(v->height + render->map->min, render->map->amplitude * cam->height + render->map->min,
                            FDF_COLOR_MIN, FDF_COLOR_MAX);
    p = (t_vec4f){pos.x, v->height, pos.y, 1.0};
    p = mat4_mul_vec4(*mvp, p);
    if (p.w < -p.z)
        return (v->ok = 0);
    v->point.x = (1 - p.x / p.w) * 0.5 * render->window->w;
    v->point.y = (1 - p.y / p.w) * 0.5 * render->window->w;
    v->z = p.z;
    return (v->ok = 1);
}

uint32_t fdf_shader_lerp(t_fdf_renderer *render, t_vec2 p, int s[2], void *data)
{
    t_fdf_vertex **vs;
    float z;
    float *depth;

    vs = (t_fdf_vertex **)data;
    z = fdf_lerp(s[0], s[1], vs[0]->z, vs[1]->z);
    depth = &render->depth[p.y * render->window->w + p.x];
    if (z < *depth)
    {
        *depth = z;
        return (fdf_lerp_rgb(s[0], s[1], vs[0]->color, vs[1]->color));
    }
    return (0);
}

void fdf_draw_vertex(t_fdf_renderer *render, t_fdf_vertex *a, t_fdf_vertex *b)
{
    t_vec2 p[2];
    t_fdf_vertex *vs[2];

    if (!a || !b || !a->ok || !b->ok)
        return;
    p[0] = a->point;
    p[1] = b->point;
    vs[0] = a;
    vs[1] = b;
    fdf_draw_segment(render, p, fdf_shader_lerp, vs);
}

void fdf_draw_map(t_fdf_renderer *render, t_fdf_camera *cam)
{
    int x;
    int y;
    int i;
    t_mat4 *mvp;
    t_fdf_vertex *v;

    y = 0;
    mvp = fdf_camera_mvp(cam);
    while (y < render->map->h)
    {
        x = 0;
        while (x < render->map->w)
        {
            fdf_vertex_make(render, cam, mvp, (t_vec2){x, y});
            x++;
        }
        y++;
    }
    i = 0;
    while (i < render->map->w * render->map->h)
    {
        v = &render->vs[i++];
        if (v->pos.x + 1 < render->map->w)
            fdf_draw_vertex(render, v, &render->vs[v->pos.y * render->map->w + (v->pos.x + 1)]);
        if (v->pos.y + 1 < render->map->h)
            fdf_draw_vertex(render, v, &render->vs[(v->pos.y + 1) * render->map->w + v->pos.x]);
    }
}
