#include "camera.h"
#include "map.h"
#include "util.h"
#include "window.h"
#include <math.h>

t_fdf_camera fdf_camera_new(float fov, t_fdf_window *window)
{
    t_fdf_camera cam;

    cam = (t_fdf_camera){0};
    cam.height = 1;
    cam.scale = (t_vec3f){1,1,1};
    cam.persp.fov = fov * PI / 180;
    cam.persp.ratio = ((float)window->w) / ((float)window->h);
    cam.persp.znear = 0.1;
    cam.persp.zfar = 100.0;
    cam.m = mat4_identity();
    cam.v = mat4_identity();
    cam.p = mat4_perspective(cam.persp);
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
        /* cam->m = mat4_translation(cam->m, (t_vec3f){-cam->pos.x, -cam->pos.y, -cam->pos.z}); */
        cam->m = mat4_rotation(cam->m, cam->rot);
        /* cam->m = mat4_translation(cam->m, (t_vec3f){+cam->pos.x, +cam->pos.y, +cam->pos.z}); */
        cam->m = mat4_scale(cam->m, cam->scale);

        cam->mvp = mat4_mul_mat4(cam->mvp, cam->p);
        cam->mvp = mat4_mul_mat4(cam->mvp, cam->v);
        cam->mvp = mat4_mul_mat4(cam->mvp, cam->m);
    }
    cam->obsolete = 0;
    return (&cam->mvp);
}

void fdf_camera_update(t_fdf_camera *cam, t_fdf_window *window)
{
    t_vec2 mouse;
    t_vec2 motion;

    if (cam->input.type == CAM_IDLE)
        return;
    mouse = fdf_window_mouse_get(window);
    motion.x = (cam->input.anchor.x - mouse.x) * FDF_CAM_SENSITIVITY;
    motion.y = (cam->input.anchor.y - mouse.y) * FDF_CAM_SENSITIVITY;
    if (cam->input.type == CAM_MOVE)
        fdf_camera_move(cam, motion);
    else if (cam->input.type == CAM_LOOK)
        fdf_camera_look(cam, motion);
    else if (cam->input.type == CAM_ZOOM)
        fdf_camera_zoom(cam, vec2_len(motion));
}

void fdf_camera_move(t_fdf_camera *cam, t_vec2 motion)
{
    if (!motion.x && !motion.y)
        return;
    cam->pos.x = cam->input.prev.x + motion.x;
    cam->pos.y = cam->input.prev.y;
    cam->pos.z = cam->input.prev.z + motion.y;
    cam->obsolete = 1;
}

void fdf_camera_look(t_fdf_camera *cam, t_vec2 motion)
{
    if (!motion.x && !motion.y)
        return;
    cam->rot.x = cam->input.prev.x + motion.y * PI / 180;
    cam->rot.y = cam->input.prev.y + motion.x * PI / 180;
    cam->rot.z = cam->input.prev.z;
    cam->obsolete = 1;
}

void fdf_camera_zoom(t_fdf_camera *cam, float amount)
{
    if (!amount)
        return;
    cam->scale.x = fmaxf(cam->scale.x + amount, 0.0);
    cam->scale.y = fmaxf(cam->scale.y + amount, 0.0);
    cam->scale.z = fmaxf(cam->scale.z + amount, 0.0);
    cam->obsolete = 1;
}

/* void fdf_camera_translate(t_fdf_camera *cam, t_vec3f motion) */
/* { */
/*     if (vec3f_null(motion)) */
/*         return; */
/*     cam->pos.x += motion.x; */
/*     cam->pos.y += motion.y; */
/*     cam->pos.z += motion.z; */
/*     cam->obsolete = 1; */
/* } */

/* void fdf_camera_rotate(t_fdf_camera *cam, t_vec3f motion) */
/* { */
/*     if (vec3f_null(motion)) */
/*         return; */
/*     cam->rot.x = fmod(cam->rot.x - motion.x, 2 * PI); */
/*     cam->rot.y = fmod(cam->rot.y - motion.y, 2 * PI); */
/*     cam->rot.z = 0; */
/*     cam->obsolete = 1; */
/* } */

/* void fdf_camera_zoom(t_fdf_camera *cam, float amount) */
/* { */
/*     if (amount != 0) */
/*     { */
/*         cam->zoom = fmaxf(cam->zoom + amount, 0.0); */
/*         cam->obsolete = 1; */
/*     } */
/* } */
