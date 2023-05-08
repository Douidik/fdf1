#include "fdf.h"
#include "map.h"
#include "render.h"
#include "util.h"
#include "window.h"
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <ft_printf.h>
#include <libft.h>
#include <math.h>
#include <mlx.h>
#include <stdlib.h>

int fdf_key_hook(int k, t_fdf *fdf);
int fdf_frame_hook(t_fdf *fdf);

t_fdf *fdf_new(t_fdf_map *map, const char *fp)
{
    t_fdf *fdf;

    fdf = ft_calloc(1, sizeof(t_fdf));
    if (!fdf)
        return (NULL);
    fdf->filename = fdf_filename(fp);
    fdf->map = map;
    fdf->mlx = mlx_init();
    if (!fdf->mlx)
        return (ft_printf("Cannot initialize mlx library\n"), fdf_free(fdf));
    fdf->window = fdf_window_new(fdf->mlx, 1280, 1000, fdf->filename);
    if (!fdf->window)
        return (ft_printf("Cannot open the window\n"), fdf_free(fdf));
    fdf->render = fdf_renderer_new(fdf->mlx, fdf->window, map);
    if (!fdf->render)
        return (ft_printf("Cannot create the renderer\n"), fdf_free(fdf));
    fdf->cam = fdf_camera_new(120, fdf->window);
    fdf->ui_tab = FDF_HELP;
    return (fdf);
}

t_fdf *fdf_free(t_fdf *fdf)
{
    if (fdf != NULL)
    {
        if (fdf->window != NULL)
            fdf_window_free(fdf->window);
        if (fdf->render != NULL)
            fdf_renderer_free(fdf->render);
        free(fdf);
    }
    return (NULL);
}


int fdf_on_window_exposed(t_fdf *fdf)
{
    fdf_renderer_free(fdf->render);
    fdf->render = fdf_renderer_new(fdf->mlx, fdf->window, fdf->map);
    fdf->cam.obsolete = 1;
    return (0);
}

void fdf_run(t_fdf *fdf)
{
    mlx_do_key_autorepeatoff(fdf->mlx);
    mlx_loop_hook(fdf->mlx, &fdf_frame_hook, fdf);
    mlx_hook(fdf->window->impl, KeyPress, KeyPressMask, &fdf_camera_keypress_hook, fdf);
    mlx_hook(fdf->window->impl, KeyRelease, KeyReleaseMask, &fdf_camera_keyrelease_hook, fdf);
    mlx_hook(fdf->window->impl, Expose, ExposureMask, &fdf_on_window_exposed, fdf);

    mlx_mouse_hook(fdf->window->impl, fdf_camera_buttonpress_hook, fdf);
    mlx_loop(fdf->mlx);
}

#include <stdio.h>
#include <time.h>

int fdf_frame_hook(t_fdf *fdf)
{
    t_fdf_ui ui;

    fdf_camera_update(&fdf->cam, fdf->window);
    if (!fdf->cam.obsolete)
        return (0);
    /* XGetWindowAttributes(fdf->window */

    
    fdf_render_clear(fdf->render);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    fdf_draw_map(fdf->render, &fdf->cam);
    fdf_draw_cam(fdf->render, &fdf->cam);

    clock_gettime(CLOCK_MONOTONIC, &end);

    unsigned long start_ms = start.tv_sec * 1E3 + start.tv_nsec / 1E6;
    unsigned long end_ms = end.tv_sec * 1E3 + end.tv_nsec / 1E6;
    unsigned long elapsed_ms = end_ms - start_ms;

    fdf_render_image(fdf->render);
    ui = (t_fdf_ui){.x = 8, .y = 16, .h = 16};
    if (fdf->ui_tab == FDF_HELP)
    {
        ui = (t_fdf_ui){.x = 8, .y = 16, .h = 16};
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "FDF - Wireframe rasterizer");
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "[Escape] Exit");
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "[W/S] Move Forward/Backward");
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "[A/D] Move Left/Right");
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "[</>] Turn Right/Left");
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "[^/v] Turn Up/Down");
        fdf_draw_ui(fdf->render, &ui, 0xffffff, "[Tab] Show Debug UI");
    }
    else if (fdf->ui_tab == FDF_DEBUG)
    {
        char benchmark[64];
        if (elapsed_ms > 0)
            snprintf(benchmark, sizeof(benchmark), "%ld ms | %.1f fps", elapsed_ms, 1000.0f / elapsed_ms);
        else
            ft_strlcpy(benchmark, "< 0 ms | > 1000 fps", sizeof(benchmark));
        char transform[128];
        snprintf(transform, sizeof(transform), "P: (%.1f, %.1f, %.1f), R: (%.1f, %.1f, %.1f), Z: %.1f", fdf->cam.pos.x,
                 fdf->cam.pos.y, fdf->cam.pos.z, fdf->cam.rot.x, fdf->cam.rot.y, fdf->cam.rot.z, fdf->cam.scale.x);
        t_fdf_camera_input *input = &fdf->cam.input;
        char input_info[128];
        snprintf(input_info, sizeof(input_info), "Type: %d, Prev: (%.1f, %.1f, %.1f), Anchor: (%d, %d), Key: %d, Height: %.1f",
                 input->type, input->prev.x, input->prev.y, input->prev.z, input->anchor.x, input->anchor.y,
                 input->key, fdf->cam.height);

        fdf_draw_ui(fdf->render, &ui, fdf_lerp_rgb(elapsed_ms, 300, 0x00ff00, 0xff0000), benchmark);
        fdf_draw_ui(fdf->render, &ui, 0xffffff, transform);
        fdf_draw_ui(fdf->render, &ui, 0xffffff, input_info);
	if (fdf->render->technique == FDF_SEGMENT_NAIVE)
	    fdf_draw_ui(fdf->render, &ui, 0xffffff, "Technique: Naive");
	if (fdf->render->technique == FDF_SEGMENT_BRESSENHAM)
	    fdf_draw_ui(fdf->render, &ui, 0xffffff, "Technique: Bressenham");
	if (fdf->render->technique == FDF_SEGMENT_ANTIALIASED)
	    fdf_draw_ui(fdf->render, &ui, 0xffffff, "Technique: Antialiased");
    }
    return (0);
}

/* int fdf_mouse_hook(int x, int y, t_fdf *fdf) */
/* { */
/*     t_vec3f motion; */

/*     motion.x = (x - fdf->mouse_x) * FDF_CAM_SENSITIVITY; */
/*     motion.y = (y - fdf->mouse_y) * FDF_CAM_SENSITIVITY; */
/*     motion.z = 0.0f; */
/*     /\* fdf_camera_rotate(&fdf->camera, motion); *\/ */
/*     fdf->mouse_x = x; */
/*     fdf->mouse_y = y; */
/*     return (0); */
/* } */

/* int fdf_frame_hook(t_fdf *fdf) */
/* { */
/*     t_fdf_ui help_ui; */

/*     if (!fdf->camera.obsolete) */
/* 	return (0); */
/*     fdf_render_clear(fdf->render); */
/*     fdf_draw_map(fdf->render, fdf->map, &fdf->camera); */
/*     fdf_render_image(fdf->render); */
/*     if (fdf->show_help) */
/*     { */
/*         help_ui = (t_fdf_ui){.x = 8, .y = 16, .h = 16}; */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "FDF - Wireframe rasterizer <Help>"); */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[Escape] Exit"); */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[W/S] Move Forward/Backward"); */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[A/D] Move Left/Right"); */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[</>] Turn Right/Left"); */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[^/v] Turn Up/Down"); */
/*         fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[Tab] Show Debug UI"); */
/*     } */
/*     return (0); */
/* } */
