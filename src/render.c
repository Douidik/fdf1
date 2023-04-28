#include "render.h"
#include "window.h"
#include "util.h"
#include <libft.h>
#include <mlx.h>
#include <stdlib.h>

t_fdf_renderer *fdf_renderer_new(void *mlx, t_fdf_window *window)
{
    t_fdf_renderer *render;

    render = ft_calloc(1, sizeof(t_fdf_renderer));
    if (!render)
        return (NULL);
    render->mlx = mlx;
    render->window = window;
    render->image = mlx_new_image(mlx, window->w, window->h);
    if (!render->image)
        return (fdf_renderer_free(render));
    render->stream = mlx_get_data_addr(render->image, &render->bpp, &render->w, (int *)&render->endian);
    if (!render->stream)
        return (fdf_renderer_free(render));
    return (render);
}

t_fdf_renderer *fdf_renderer_free(t_fdf_renderer *render)
{
    if (render != NULL)
    {
        if (render->image != NULL)
            mlx_destroy_image(render->mlx, render->image);
        free(render);
    }
    return (NULL);
}

/* void fdf_draw_pixel(t_fdf_renderer *render, t_vec2 pos, int rgb) */
/* { */
/*     size_t n; */
/*     int *dst; */

/*     if (pos.x >= 0 && pos.x < render->window->w && pos.y >= 0 && pos.y < render->window->h) */
/*     { */
/* 	n = (pos.y * render->w + pos.x * 4); */
/* 	if (render->endian == FDF_ENDIAN_BIG) */
/* 	{ */
/* 	    render->stream[n + 0] = (rgb >> 0x18) & 255; */
/* 	    render->stream[n + 1] = (rgb >> 0x10) & 255; */
/* 	    render->stream[n + 2] = (rgb >> 0x08) & 255; */
/* 	    render->stream[n + 3] = (rgb >> 0x00) & 255; */
/* 	} */
/* 	if (render->endian == FDF_ENDIAN_SMALL) */
/* 	{ */
/* 	    render->stream[n + 0] = (rgb >> 0x00) & 255; */
/* 	    render->stream[n + 1] = (rgb >> 0x08) & 255; */
/* 	    render->stream[n + 2] = (rgb >> 0x10) & 255; */
/* 	    render->stream[n + 3] = (rgb >> 0x18) & 255; */
/* 	} */
/*     } */
/* } */

void fdf_draw_pixel(t_fdf_renderer *render, t_vec2 pos, int rgb)
{
    size_t n;
    int *dst;

    n = (pos.y * render->w + pos.x * (render->bpp / 8));
    dst = (int *)(render->stream + n);
    rgb = mlx_get_color_value(render->mlx, rgb);
    *dst = (*dst) ^ ((*dst ^ rgb) & 0xffffff);
}

void fdf_render_clear(t_fdf_renderer *render)
{
    fdf_memset(render->stream, 0, render->window->w * render->window->h * (render->bpp / 8));
}

void fdf_render_image(t_fdf_renderer *render)
{
    mlx_put_image_to_window(render->mlx, render->window->impl, render->image, 0, 0);
}
