#include "camera.h"
#include "util.h"
#include "window.h"
#include <math.h>

t_fdf_camera fdf_camera_new(float fov, t_fdf_window *window)
{
    t_fdf_camera camera;

    camera = (t_fdf_camera){0};
    camera.w = &window->w;
    camera.h = &window->h;
    camera.persp.fov = fov * PI / 180;
    camera.persp.ratio = ((float)*camera.w) / ((float)*camera.h);
    camera.persp.znear = 0.1;
    camera.persp.zfar = 100.0;
    camera.m = mat4_identity();
    camera.v = mat4_identity();
    camera.p = mat4_perspective(camera.persp);
    camera.zoom = 1;
    camera.obsolete = 1;
    return (camera);
}

t_mat4 *fdf_camera_mvp(t_fdf_camera *camera)
{
    if (camera->obsolete)
    {
        camera->mvp = mat4_identity();
        camera->m = mat4_identity();
        camera->v = mat4_identity();
	
        camera->v = mat4_translation(camera->v, camera->position);
        camera->m = mat4_rotation(camera->m, camera->rotation);
        camera->m = mat4_scale(camera->m, (t_vec3f){camera->zoom, camera->zoom, camera->zoom});
	
        camera->p = mat4_perspective(camera->persp);
        camera->mvp = mat4_mul_mat4(camera->mvp, camera->p);
        camera->mvp = mat4_mul_mat4(camera->mvp, camera->v);
        camera->mvp = mat4_mul_mat4(camera->mvp, camera->m);
    }
    camera->obsolete = 0;
    return (&camera->mvp);
}

void fdf_camera_translate(t_fdf_camera *camera, t_vec3f motion)
{
    if (!vec3f_null(motion))
    {
        camera->position.x += motion.x;
        camera->position.y += motion.y;
        camera->position.z += motion.z;
        camera->obsolete = 1;
    }
}

void fdf_camera_rotate(t_fdf_camera *camera, t_vec3f motion)
{
    if (!vec3f_null(motion))
    {
	camera->rotation.x = fmod(camera->rotation.x - motion.x, 2 * PI);
	camera->rotation.y = fmod(camera->rotation.y - motion.y, 2 * PI);
        camera->rotation.z = 0;
	camera->obsolete = 1;
    }
}

void fdf_camera_zoom(t_fdf_camera *camera, float amount)
{
    if (amount != 0)
    {
        camera->zoom += amount;
	camera->obsolete = 1;
    }
}
