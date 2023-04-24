#include "render.h"
#include "fdf.h"
#include "map.h"
#include "window.h"
#include "camera.h"

void fdf_draw_debug_map(t_fdf_renderer *render, t_fdf_ui *ui, t_fdf_map *map)
{
}

void fdf_draw_debug(t_fdf_renderer *render, t_fdf *fdf)
{
	t_fdf_ui ui;

	ui = (t_fdf_ui) {.x = 8, .y = 16, .h = 16, .w = 256, .pad_x = 2, .pad_y = 2};
	fdf_draw_ui(fdf->render, &ui, 0x2596be, "FDF - Wireframe rasterizer <Debug>");
	fdf_draw_ui_boxed(fdf->render, &ui, 0x2596be, fdf->filename);
}
