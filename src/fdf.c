#include "fdf.h"
#include "map.h"
#include "render.h"
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
    fdf->map = map;
    fdf->mlx = mlx_init();
    if (!fdf->mlx)
        return (ft_printf("Cannot initialize mlx library\n"), fdf_free(fdf));
    fdf->window = fdf_window_new(fdf->mlx, 1280, 720, fp);
    if (!fdf->window)
        return (ft_printf("Cannot open the window\n"), fdf_free(fdf));
    fdf->render = fdf_renderer_new(fdf->mlx, fdf->window);
    if (!fdf->render)
        return (ft_printf("Cannot create the renderer\n"), fdf_free(fdf));
    fdf->camera = fdf_camera_new(90, fdf->window);
    fdf->show_ui = 1;
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
        mov.z--;
    if (k == XK_s)
        mov.z++;
    if (k == XK_a)
        mov.x++;
    if (k == XK_d)
        mov.x--;

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
        fdf->show_ui = !fdf->show_ui;

    mov.y = (k == XK_space) - (k == XK_c);
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

int fdf_frame_hook(t_fdf *fdf)
{
    if (fdf->camera.obsolete)
    {
        fdf_draw_map(fdf->render, fdf->map, &fdf->camera);
    }
    fdf_render_image(fdf->render);
    if (fdf->show_ui)
    {
	fdf_draw_ui(fdf->render, 0xffffff, "FDF - Wireframe rasterizer");
	fdf_draw_ui(fdf->render, 0xffffff, "[Escape] Exit");
	fdf_draw_ui(fdf->render, 0xffffff, "[W/S] Move Forward/Backward");
	fdf_draw_ui(fdf->render, 0xffffff, "[A/D] Move Left/Right");
	fdf_draw_ui(fdf->render, 0xffffff, "[</>] Turn Right/Left");
	fdf_draw_ui(fdf->render, 0xffffff, "[^/v] Turn Up/Down");
	fdf_draw_ui(fdf->render, 0xffffff, "[Tab] Hide/Show UI");
    }
    return (0);
}
