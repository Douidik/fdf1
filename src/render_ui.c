#include "render.h"
#include "window.h"
#include <mlx.h>

void fdf_draw_ui(t_fdf_renderer *render, int color, char *str)
{
    int x;
    int y;

    x = render->pos_ui.x;
    y = render->pos_ui.y;
    mlx_string_put(render->mlx, render->window->impl, x, y, color, str);
    render->pos_ui.y += 16;
}
