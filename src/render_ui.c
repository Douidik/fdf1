#include "render.h"
#include "window.h"
#include <mlx.h>

void fdf_draw_ui(t_fdf_renderer *render, t_fdf_ui *ui, int rgb, char *str)
{
    mlx_string_put(render->mlx, render->window->impl, ui->x + ui->pad_x, ui->y + ui->pad_y, rgb, str);
	ui->y = ui->h + ui->pad_y;
}

void fdf_draw_ui_boxed(t_fdf_renderer *render, t_fdf_ui *ui, int rgb, char *str)
{
	t_vec2 pos;

	pos = (t_vec2) {.x = ui->x, .y = ui->y};
	while (pos.x < ui->x + ui->w + ui->pad_x * 2)
		fdf_draw_pixel(render, pos, rgb);
	while (pos.y < ui->y + ui->h + ui->pad_y * 2)
		fdf_draw_pixel(render, pos, rgb);
	pos = (t_vec2) {.x = ui->x, .y = ui->y};
	while (pos.y < ui->y + ui->h + ui->pad_y * 2)
		fdf_draw_pixel(render, pos, rgb);
	while (pos.x < ui->x + ui->w + ui->pad_x * 2)
		fdf_draw_pixel(render, pos, rgb);
	fdf_draw_ui(render, ui, rgb, str);
}
