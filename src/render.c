#include "render.h"
#include "window.h"
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

void fdf_draw_pixel(t_fdf_renderer *render, t_vec2 pos, int rgb)
{
    size_t dst;

    if ((pos.x >= 0 && pos.x < render->window->w) && (pos.y >= 0 && pos.y < render->window->h))
    {
        dst = pos.y * render->w + pos.x * (render->bpp / 8);
        (*(int *)(render->stream + dst)) = mlx_get_color_value(render->mlx, rgb);
    }
}

void fdf_render_image(t_fdf_renderer *render)
{
    mlx_clear_window(render, render->window->impl);
    mlx_put_image_to_window(render->mlx, render->window->impl, render->image, 0, 0);
    render->pos_ui = (t_vec2){16, 16};
}
