#include "camera.h"
#include "fdf.h"
#include "render.h"
#include "window.h"
#include <X11/keysym.h>
#include <mlx.h>

void fdf_camera_input_make(t_fdf *fdf, t_fdf_camera_input_type type, t_vec3f prev, int key)
{
    t_fdf_camera_input *input;

    input = &fdf->cam.input;
    input->prev = prev;
    input->anchor = fdf_window_mouse_get(fdf->window);
    input->type = type;
    input->key = key;

    mlx_mouse_hide(fdf->window->mlx, fdf->window->impl);
}

int fdf_camera_keypress_hook(int k, t_fdf *fdf)
{
    t_fdf_camera *cam;

    cam = &fdf->cam;
    if (k == XK_Tab)
    {
        fdf->ui_tab = (fdf->ui_tab + 1) % FDF_UI_TAB_COUNT;
        cam->obsolete = 1;
    }
    else if (k == XK_F1)
        fdf->render->technique = FDF_SEGMENT_NAIVE, cam->obsolete = 1;
    else if (k == XK_F2)
        fdf->render->technique = FDF_SEGMENT_BRESSENHAM, cam->obsolete = 1;
    else if (k == XK_F3)
        fdf->render->technique = FDF_SEGMENT_ANTIALIASED, cam->obsolete = 1;
    else if (k == XK_Escape)
	mlx_loop_end(fdf->mlx);
    if (fdf->cam.input.type != CAM_IDLE)
        return (0);
    else if (k == XK_q)
        fdf_camera_input_make(fdf, CAM_MOVE, cam->pos, XK_q);
    else if (k == XK_w)
        fdf_camera_input_make(fdf, CAM_LOOK, cam->rot, XK_w);
    else if (k == XK_e)
        fdf_camera_input_make(fdf, CAM_ZOOM, cam->scale, XK_e);
    return (0);
}

int fdf_camera_keyrelease_hook(int k, t_fdf *fdf)
{
    t_fdf_camera_input *input;

    input = &fdf->cam.input;
    if (input->key == k && input->type != CAM_IDLE)
    {
        mlx_mouse_show(fdf->window->mlx, fdf->window->impl);
        input->type = CAM_IDLE;
        fdf->cam.obsolete = 1;
    }
    return (0);
}

void fdf_camera_height(t_fdf_camera *cam, float delta)
{
    cam->height += delta;
    if (cam->height == -0.0)
        cam->height += delta;
    cam->obsolete = 1;
}

int fdf_camera_buttonpress_hook(int b, int x, int y, t_fdf *fdf)
{
    if (b == 4)
        fdf_camera_height(&fdf->cam, +0.25);
    else if (b == 5)
        fdf_camera_height(&fdf->cam, -0.25);
    return (0);
}
