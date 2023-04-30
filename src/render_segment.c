#include "cohen_sutherland.h"
#include "render.h"
#include "window.h"
#include <math.h>

void fdf_draw_segment_fast(t_fdf_renderer *render, t_vec2 points[2], int (*shader)(int, int, void *), void *data)
{
    t_vec2f d;
    t_vec2f p;
    int step;
    int s;

    if (!cohen_sutherland(&points[0], &points[1], render->window->w, render->window->h))
        return;
    d.x = (points[1].x - points[0].x);
    d.y = (points[1].y - points[0].y);
    step = fmaxf(fabsf(d.x), fabsf(d.y));
    if (step != 0)
    {
        d.x /= step;
        d.y /= step;
    }
    p = (t_vec2f){points[0].x, points[0].y};
    s = 0;
    while (s++ < step)
    {
        fdf_draw_pixel_rgb(render, (t_vec2){p.x, p.y}, shader(s, step, data));
        p.x += d.x;
        p.y += d.y;
    }
}

void fdf_draw_segment_antialiased(t_fdf_renderer *render, t_vec2 points[2], int (*shader)(int, int, void *), void *data)
{
    t_vec2f d;
    t_vec2f p;
    int step;
    int s;
    int color;

    if (!cohen_sutherland(&points[0], &points[1], render->window->w, render->window->h))
        return;
    d.x = (points[1].x - points[0].x);
    d.y = (points[1].y - points[0].y);
    step = fmaxf(fabsf(d.x), fabsf(d.y));
    if (step != 0)
    {
        d.x /= step;
        d.y /= step;
    }
    p = (t_vec2f){points[0].x, points[0].y};
    s = 0;
    while (s++ < step)
    {
        color = shader(s, step, data);
        fdf_draw_pixel_rgb(render, (t_vec2){p.x, p.y}, color);
        if (p.x + d.x >= 0 && p.x + d.x < render->window->w)
            fdf_draw_pixel_rgba(render, (t_vec2){p.x + d.x, p.y}, color, p.x - (long)p.x);
        if (p.x - d.x >= 0 && p.x - d.x < render->window->w)
            fdf_draw_pixel_rgba(render, (t_vec2){p.x - d.x, p.y}, color, 1.0 - (p.x - (long)p.x));
        if (p.y + d.y >= 0 && p.y + d.y < render->window->h)
            fdf_draw_pixel_rgba(render, (t_vec2){p.x, p.y + d.y}, color, p.y - (long)p.y);
        if (p.y - d.y >= 0 && p.y - d.y < render->window->h)
            fdf_draw_pixel_rgba(render, (t_vec2){p.x, p.y - d.y}, color, 1.0 - (p.y - (long)p.y));
        p.x += d.x;
        p.y += d.y;
    }
}
