#include "render.h"
#include "window.h"

void fdf_draw_pixel(t_fdf_renderer *render, t_vec2 pos, uint32_t color)
{
    if (color >> 24 != 255)
        return fdf_draw_pixel_rgba(render, pos, color & 0xffffff, (float)(color >> 24) / 255);
    *(int *)(render->stream + pos.y * render->w + pos.x * (render->bpp / 8)) = color;
}

void fdf_draw_pixel_rgba(t_fdf_renderer *render, t_vec2 pos, uint32_t rgb, float alpha)
{
    size_t n;
    uint32_t *dst;
    uint32_t r;
    uint32_t g;
    uint32_t b;

    if (alpha == 0)
	return;

    n = (pos.y * render->w + pos.x * (render->bpp / 8));
    dst = (uint32_t *)(render->stream + n);
    r = (1.0 - alpha) * (*dst & 255) + alpha * (rgb & 255);
    g = (1.0 - alpha) * (*dst >> 8 & 255) + alpha * (rgb >> 8 & 255);
    b = (1.0 - alpha) * (*dst >> 16 & 255) + alpha * (rgb >> 16 & 255);
    *dst = r | (g << 8) | (b << 16);
}
