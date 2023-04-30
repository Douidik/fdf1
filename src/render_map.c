#include "camera.h"
#include "cohen_sutherland.h"
#include "map.h"
#include "render.h"
#include "util.h"
#include "window.h"
#include <libft.h>
#include <math.h>
#include <mlx.h>

#define FDF_COLOR_MIN (0x6666ee)
#define FDF_COLOR_MAX (0xee22ee)

t_fdf_vertex fdf_vertex_make(t_fdf_map *map, t_fdf_camera *cam, t_mat4 *mvp, t_vec2 pos)
{
    t_vec4f v;
    t_vec2 p;
    int height;
    int color;
    float dist;

    p = (t_vec2){0};
    if (pos.x >= map->w || pos.y >= map->h)
        return (t_fdf_vertex){.ok = 0};
    height = map->buf[pos.y * map->w + pos.x];
    color = fdf_lerp_rgb(height + map->min, map->amplitude + map->min, FDF_COLOR_MIN, FDF_COLOR_MAX);
    v.x = pos.x;
    v.y = height;
    v.z = pos.y;
    v.w = 1.0;
    v = mat4_mul_vec4(*mvp, v);
    if (v.z < -v.w)
        return (t_fdf_vertex){.ok = 0};
    p.x = (1 - v.x / v.w) * 0.5 * (*cam->w);
    p.y = (1 - v.y / v.w) * 0.5 * (*cam->h);
    dist = powf(v.x + cam->pos.x, 2) + powf(v.y + cam->pos.y, 2) + powf(v.z + cam->pos.z, 2);
    return (t_fdf_vertex){1, color, p, (t_vec2){pos.x, pos.y}, dist};
}

#include <stdlib.h>

int vertex_dist_cmp(const void *v_, const void *w_)
{
    const t_fdf_vertex *v = *(t_fdf_vertex **)v_;
    const t_fdf_vertex *w = *(t_fdf_vertex **)w_;

    return (w->dist - v->dist);
}

t_fdf_vertex **fdf_sort_vertices(t_fdf_renderer *render, t_fdf_map *map)
{
    t_fdf_vertex *tmp;
    size_t i;
    size_t len;

    len  = map->w * map->h;
    i = 0;
    while (i < len)
    {
        render->vss[i] = &render->vs[i];
        i++;
    }
    return (fdf_qsort(render->vss, len, sizeof(t_fdf_vertex *), vertex_dist_cmp));
}

int fdf_shader_lerp(int s, int step, void *data)
{
    return (fdf_lerp_rgb(s, step, ((int *)data)[0], ((int *)data)[1]));
}

void fdf_draw_vertex(t_fdf_renderer *render, t_fdf_vertex *a, t_fdf_vertex *b)
{
    t_vec2 points[2];
    int colors[2];

    if (!a || !b || !a->ok || !b->ok)
        return;
    points[0] = a->point;
    points[1] = b->point;
    colors[0] = a->color;
    colors[1] = b->color;
    fdf_draw_segment_fast(render, points, fdf_shader_lerp, colors);
}

void fdf_draw_map(t_fdf_renderer *render, t_fdf_map *map, t_fdf_camera *cam)
{
    int x;
    int y;
    size_t i;
    t_mat4 *mvp;
    t_fdf_vertex *v;

    y = 0;
    mvp = fdf_camera_mvp(cam);
    while (y < map->h)
    {
        x = 0;
        while (x < map->w)
        {
            render->vs[y * map->w + x] = fdf_vertex_make(map, cam, mvp, (t_vec2){x, y});
            x++;
        }
        y++;
    }
    render->vss = fdf_sort_vertices(render, map);
    i = 0;
    while (i < map->w * map->h)
    {
	v = render->vss[i];
	if (v->pos.x + 1 < map->w)
	    fdf_draw_vertex(render, v, &render->vs[v->pos.y * map->w + (v->pos.x + 1)]);
	if (v->pos.y + 1 < map->h)
	    fdf_draw_vertex(render, v, &render->vs[(v->pos.y + 1) * map->w + v->pos.x]);
        i++;
    }
}
