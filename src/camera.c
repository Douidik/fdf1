#include "camera.h"
#include "util.h"
#include "window.h"
#include <math.h>

t_fdf_camera fdf_camera_new(float fov, t_fdf_window *window)
{
    t_fdf_camera cam;

    cam = (t_fdf_camera){0};
    cam.w = &window->w;
    cam.h = &window->h;
    cam.persp.fov = fov * PI / 180;
    cam.persp.ratio = ((float)*cam.w) / ((float)*cam.h);
    cam.persp.znear = 0.1;
    cam.persp.zfar = 100.0;
    cam.m = mat4_identity();
    cam.v = mat4_identity();
    cam.p = mat4_perspective(cam.persp);
    cam.zoom = 1;
    cam.obsolete = 1;
    return (cam);
}

t_mat4 *fdf_camera_mvp(t_fdf_camera *cam)
{
    if (cam->obsolete)
    {
        cam->mvp = mat4_identity();
        cam->m = mat4_identity();
        cam->v = mat4_identity();
	cam->p = mat4_perspective(cam->persp);

	cam->v = mat4_translation(cam->v, cam->pos);
        cam->m = mat4_rotation(cam->m, cam->rot);
        cam->m = mat4_scale(cam->m, (t_vec3f){cam->zoom, cam->zoom, cam->zoom});
	
	
        /* cam->m = mat4_scale(cam->m, (t_vec3f){cam->zoom, cam->zoom, cam->zoom}); */
        /* cam->v = mat4_translation(cam->v, cam->pos); */
        /* cam->v = mat4_rotation(cam->v, cam->rot); */

        cam->mvp = mat4_mul_mat4(cam->mvp, cam->p);
        cam->mvp = mat4_mul_mat4(cam->mvp, cam->v);
        cam->mvp = mat4_mul_mat4(cam->mvp, cam->m);
    }
    cam->obsolete = 0;
    return (&cam->mvp);
}

void fdf_camera_translate(t_fdf_camera *cam, t_vec3f motion)
{
    if (vec3f_null(motion))
	return;
    cam->pos.x += motion.x;
    cam->pos.y += motion.y;
    cam->pos.z += motion.z;
    cam->obsolete = 1;
}

void fdf_camera_rotate(t_fdf_camera *cam, t_vec3f motion)
{
    if (vec3f_null(motion))
	return;
    cam->rot.x = fmod(cam->rot.x - motion.x, 2 * PI);
    cam->rot.y = fmod(cam->rot.y - motion.y, 2 * PI);
    cam->rot.z = 0;
    cam->obsolete = 1;
}

void fdf_camera_zoom(t_fdf_camera *cam, float amount)
{
    if (amount != 0)
    {
        cam->zoom = fmaxf(cam->zoom + amount, 0.0);
	cam->obsolete = 1;
    }
}
