#include "fdf.h"
#include "map.h"
#include "render.h"
#include "util.h"
#include "window.h"
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
    fdf->window = fdf_window_new(fdf->mlx, 1000, 1000, fdf->filename);
    if (!fdf->window)
        return (ft_printf("Cannot open the window\n"), fdf_free(fdf));
    fdf->render = fdf_renderer_new(fdf->mlx, fdf->window);
    if (!fdf->render)
        return (ft_printf("Cannot create the renderer\n"), fdf_free(fdf));
    fdf->camera = fdf_camera_new(90, fdf->window);
    fdf->show_help = 1;
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

void fdf_run(t_fdf *fdf)
{
    mlx_do_key_autorepeaton(fdf->mlx);
    mlx_loop_hook(fdf->mlx, &fdf_frame_hook, fdf);
    mlx_hook(fdf->window->impl, KeyPress, KeyPressMask, &fdf_key_hook, fdf);
    /* mlx_hook(fdf->window->impl, MotionNotify, PointerMotionMask, &fdf_mouse_hook, fdf); */
    mlx_loop(fdf->mlx);
}

int fdf_key_hook(int k, t_fdf *fdf)
{
    t_vec3f mov;
    t_vec3f rot;
    float zoom;

    if (k == XK_Escape)
        mlx_loop_end(fdf->mlx);
    mov = (t_vec3f){0};
    rot = (t_vec3f){0};
    zoom = 0;
    if (k == XK_w)
        mov.z -= 0.5;
    if (k == XK_s)
        mov.z += 0.5;
    if (k == XK_a)
        mov.x -= 0.5;
    if (k == XK_d)
        mov.x += 0.5;

    if (k == XK_Right)
        rot.y += 0.1;
    if (k == XK_Left)
        rot.y -= 0.1;
    if (k == XK_Up)
        rot.x += 0.1;
    if (k == XK_Down)
        rot.x -= 0.1;

    if (k == XK_f)
        zoom = +1;
    if (k == XK_g)
        zoom = -1;

    if (k == XK_Tab)
    {
        fdf->show_help = !fdf->show_help;
        fdf->camera.obsolete = 1;
    }

    mov.y = (k == XK_c) - (k == XK_space);
    mov = vec3f_norm(mov);
    fdf_camera_translate(&fdf->camera, mov);
    fdf_camera_rotate(&fdf->camera, rot);
    fdf_camera_zoom(&fdf->camera, zoom);
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

#include <stdio.h>
#include <time.h>

int fdf_frame_hook(t_fdf *fdf)
{
    t_fdf_ui help_ui;

    if (!fdf->camera.obsolete)
        return (0);
    fdf_render_clear(fdf->render);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* /\* Gradient test *\/ */
    /* for (int x = 0; x < fdf->window->w; x++) */
    /* { */
    /* 	for (int y = 0; y < fdf->window->h; y++) */
    /*     { */
    /* 	    int rgb = fdf_lerp_rgb(x+y, fdf->window->w+fdf->window->h, 0, 0xff00ff); */
    /* 	    if (fdf->show_help) */
    /* 		fdf_draw_pixel(fdf->render, (t_vec2){x,y}, rgb); */
    /* 	    else */
    /* 		mlx_pixel_put(fdf->mlx, fdf->window->impl, x, y, rgb); */
    /*     } */
    /* } */

    fdf_draw_map(fdf->render, fdf->map, &fdf->camera);
    clock_gettime(CLOCK_MONOTONIC, &end);

    unsigned long start_ms = start.tv_sec * 1E3 + start.tv_nsec / 1E6;
    unsigned long end_ms = end.tv_sec * 1E3 + end.tv_nsec / 1E6;
    unsigned long elapsed_ms = end_ms - start_ms;
    char benchmark[64];
    if (elapsed_ms > 0)
        snprintf(benchmark, sizeof(benchmark), "%ld ms | %.1f fps", elapsed_ms, 1000.0f / elapsed_ms);
    else
        ft_strlcpy(benchmark, "< 0 ms | > 1000 fps", sizeof(benchmark));

    fdf_render_image(fdf->render);
    if (fdf->show_help)
    {
        help_ui = (t_fdf_ui){.x = 8, .y = 16, .h = 16};
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "FDF - Wireframe rasterizer <Help>");
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[Escape] Exit");
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[W/S] Move Forward/Backward");
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[A/D] Move Left/Right");
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[</>] Turn Right/Left");
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[^/v] Turn Up/Down");
        fdf_draw_ui(fdf->render, &help_ui, 0xffffff, "[Tab] Show Debug UI");

        fdf_draw_ui(fdf->render, &help_ui, fdf_lerp_rgb(elapsed_ms, 300, 0x00ff00, 0xff0000), benchmark);
    }
    return (0);
}
