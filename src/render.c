#include "render.h"
#include "map.h"
#include "util.h"
#include "window.h"
#include <libft.h>
#include <mlx.h>
#include <stdlib.h>

t_fdf_renderer *fdf_renderer_new(void *mlx, t_fdf_window *window, t_fdf_map *map)
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
    render->vs = ft_calloc(map->w * map->h, sizeof(t_fdf_vertex));
    if (!render->vs)
        return (fdf_renderer_free(render));
    render->vss = ft_calloc(map->w * map->h, sizeof(t_fdf_vertex*));
    if (!render->vss)
        return (fdf_renderer_free(render));
    return (render);
}

t_fdf_renderer *fdf_renderer_free(t_fdf_renderer *render)
{
    if (render != NULL)
    {
        if (render->image != NULL)
            mlx_destroy_image(render->mlx, render->image);
	if (render->vs != NULL)
	    free(render->vs);
	if (render->vss != NULL)
	    free(render->vss);
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

/* void fdf_draw_pixel_rgb(t_fdf_renderer *render, t_vec2 pos, int rgb) */
/* { */
/*     size_t n; */
/*     int *dst; */

/*     if (pos.x >= 0 && pos.x < render->window->w && pos.y >= 0 && pos.y < render->window->h) */
/*     { */
/* 	n = (pos.y * render->w + pos.x * 4); */
/* 	if (render->endian == FDF_ENDIAN_BIG) */
/* 	{ */
/* 	    render->stream[n + 0] = (255 >> 0x18) & 255; */
/* 	    render->stream[n + 1] = (rgb >> 0x10) & 255; */
/* 	    render->stream[n + 2] = (rgb >> 0x08) & 255; */
/* 	    render->stream[n + 3] = (rgb >> 0x00) & 255; */
/* 	} */
/* 	if (render->endian == FDF_ENDIAN_SMALL) */
/* 	{ */
/* 	    render->stream[n + 0] = (rgb >> 0x00) & 255; */
/* 	    render->stream[n + 1] = (rgb >> 0x08) & 255; */
/* 	    render->stream[n + 2] = (rgb >> 0x10) & 255; */
/* 	    render->stream[n + 3] = (255 >> 0x18) & 255; */
/* 	} */
/*     } */
/* } */

void fdf_draw_pixel_rgb(t_fdf_renderer *render, t_vec2 pos, int rgb)
{
    *(int *)(render->stream + pos.y * render->w + pos.x * (render->bpp / 8)) = rgb;
}

void fdf_draw_pixel_rgba(t_fdf_renderer *render, t_vec2 pos, int rgb, float alpha)
{
    size_t n;
    int *dst;
    int r;
    int g;
    int b;

    n = (pos.y * render->w + pos.x * (render->bpp / 8));
    dst = (int *)(render->stream + n);
    r = (1 - alpha) * (*dst & 255) + alpha * (rgb & 255);
    g = (1 - alpha) * (*dst >> 8 & 255) + alpha * (rgb >> 8 & 255);
    b = (1 - alpha) * (*dst >> 16 & 255) + alpha * (rgb >> 16 & 255);
    *dst = r | (g << 8) | (b << 16);
}

void fdf_render_clear(t_fdf_renderer *render)
{
    fdf_memset(render->stream, 0, render->window->w * render->window->h * (render->bpp / 8));
}

void fdf_render_image(t_fdf_renderer *render)
{
    mlx_put_image_to_window(render->mlx, render->window->impl, render->image, 0, 0);
}
