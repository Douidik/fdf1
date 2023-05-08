#include "render.h"
#include "window.h"
#include <mlx.h>

void fdf_draw_ui(t_fdf_renderer *render, t_fdf_ui *ui, uint32_t rgb, char *str)
{
    mlx_string_put(render->mlx, render->window->impl, ui->x, ui->y, rgb, str);
    ui->y += ui->h;
}
