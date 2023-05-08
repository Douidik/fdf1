#include "cohen_sutherland.h"
#include "render.h"
#include "util.h"
#include "window.h"
#include <math.h>

void fdf_draw_segment_naive(t_fdf_renderer *render, t_vec2 p[2], t_fdf_shader shader, void *data)
{
    t_vec2f d;
    t_vec2f v;
    int step;
    int s;
    uint32_t color;

    if (!cohen_sutherland(&p[0], &p[1], render->window->w, render->window->h))
        return;
    d.x = (p[1].x - p[0].x);
    d.y = (p[1].y - p[0].y);
    v = (t_vec2f){p[0].x, p[0].y};
    step = fmaxf(fabsf(d.x), fabsf(d.y));
    if (step != 0)
    {
        d.x /= step;
        d.y /= step;
    }
    s = 0;
    while (s < step)
    {
        color = shader(render, (t_vec2){v.x, v.y}, (int[2]){s, step}, data);
        fdf_draw_pixel(render, (t_vec2){v.x, v.y}, color);
        v.x += d.x;
        v.y += d.y;
        s++;
    }
}

void fdf_draw_segment_bressenham(t_fdf_renderer *render, t_vec2 p[2], t_fdf_shader shader, void *data)
{
    t_vec2 d;
    t_vec2 s;
    int e;
    int i;
    uint32_t color;

    if (!cohen_sutherland(&p[0], &p[1], render->window->w, render->window->h))
        return;
    d.x = +fdf_abs(p[1].x - p[0].x);
    d.y = -fdf_abs(p[1].y - p[0].y);
    s.x = p[0].x < p[1].x ? +1 : -1;
    s.y = p[0].y < p[1].y ? +1 : -1;
    e = d.x + d.y;
    i = 0;
    while (p[0].x != p[1].x || p[0].y != p[1].y)
    {
        color = shader(render, (t_vec2){p[0].x, p[0].y}, (int[2]){i++, d.x + -d.y + 1}, data);
        fdf_draw_pixel(render, (t_vec2){p[0].x, p[0].y}, color);
        int e2 = 2 * e;
        if (e2 >= d.y)
            e += d.y, p[0].x += s.x;
        if (e2 <= d.x)
            e += d.x, p[0].y += s.y;
    }
}

void fdf_draw_segment_antialiased(t_fdf_renderer *render, t_vec2 p[2], t_fdf_shader shader, void *data)
{
    t_vec2f d;
    t_vec2f v;
    int step;
    int s;
    uint32_t color;
    float f;

    if (!cohen_sutherland(&p[0], &p[1], render->window->w, render->window->h))
        return;
    d.x = (p[1].x - p[0].x);
    d.y = (p[1].y - p[0].y);
    v = (t_vec2f){p[0].x, p[0].y};
    step = fmaxf(fabsf(d.x), fabsf(d.y));
    if (step != 0)
    {
        d.x /= step;
        d.y /= step;
    }
    s = 0;
    while (s < step)
    {
        color = shader(render, (t_vec2){v.x, v.y}, (int[2]){s, step}, data);
        fdf_draw_pixel(render, (t_vec2){v.x, v.y}, color);
        float a = (float)((color >> 24) & 255) / 255;

        if (a != 0)
        {
            f = fabsf(v.y - truncf(v.y));
            if (f > 0.5)
                fdf_draw_pixel_rgba(render, (t_vec2){v.x, v.y - d.y}, color, a * f);
            else
                fdf_draw_pixel_rgba(render, (t_vec2){v.x, v.y + d.y}, color, a * (1.0 - f));
            f = fabsf(v.x - truncf(v.x));
            if (f > 0.5)
                fdf_draw_pixel_rgba(render, (t_vec2){v.x + d.x, v.y}, color, a * f);
            else
                fdf_draw_pixel_rgba(render, (t_vec2){v.x - d.x, v.y}, color, a * (1.0 - f));
        }
	
        v.x += d.x;
        v.y += d.y;
        s++;
    }
}

void fdf_draw_segment(t_fdf_renderer *render, t_vec2 p[2], t_fdf_shader shader, void *data)
{
    if (render->technique == FDF_SEGMENT_NAIVE)
        fdf_draw_segment_naive(render, p, shader, data);
    else if (render->technique == FDF_SEGMENT_BRESSENHAM)
        fdf_draw_segment_bressenham(render, p, shader, data);
    else if (render->technique == FDF_SEGMENT_ANTIALIASED)
        fdf_draw_segment_antialiased(render, p, shader, data);
}
