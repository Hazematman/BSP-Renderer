#include <stdlib.h>
#include <math.h>
#include "vec.h"
#include "mat.h"
#include "cam.h"

struct s_Cam {
	vec3Float pos;
	vec3Float fwd;
	vec3Float angle;
	vec3Float right;
	vec3Float up;
};

static void updateLookAt(Cam *cam){
	cam->angle.x += PI/2.0f;
	cam->fwd.x = sinf(cam->angle.x) * cosf(-cam->angle.y);
	cam->fwd.y = cosf(cam->angle.x);
	cam->fwd.z = sinf(cam->angle.x) * sinf(-cam->angle.y);
	cam->angle.x -= PI/2.0f;
	vec3Cross(&cam->right, &cam->fwd, &cam->up);
}

Cam *makeCam(){
	Cam *cam = malloc(sizeof(Cam));
	
	cam->pos = (vec3Float){0,0,0};
	cam->fwd = (vec3Float){0,0,-1};
	cam->angle = (vec3Float){0,0,0};
	cam->right = (vec3Float){1,0,0};
	cam->up = (vec3Float){0,1,0};
	
	return cam;
}

void moveCam(Cam *cam, float amount){
	vec3Float tmp;
	vec3Mul(&tmp, &cam->fwd, amount);
	vec3Add(&cam->pos, &cam->pos, &tmp);
}

void strafeCam(Cam *cam, float amount){
	vec3Float tmp;
	vec3Mul(&tmp, &cam->right, amount);
	vec3Add(&cam->pos, &cam->pos, &tmp);
}

void turnCam(Cam *cam, float anglex, float angley){
	cam->angle.x -= anglex;
	cam->angle.y += angley;
	cam->angle.x += PI/2.0f;
	if(cam->angle.x >= PI){
		cam->angle.x = PI;
	} else if(cam->angle.x <= 0){
		cam->angle.x = 0;
	}

	if(cam->angle.y > 2.0*PI){
		cam->angle.y = fabs(cam->angle.y)-2.0f*PI;
	} else if(cam->angle.y < -2.0f*PI){
		cam->angle.y = -(fabs(cam->angle.y)-2.0f*PI);
	}
	cam->angle.x -= PI/2.0f;
	updateLookAt(cam);
}

void getCamMatrix(Cam *cam, mat4 *mat){
	lookAt(mat, &cam->pos, &cam->fwd, &cam->right, &cam->up);
}

void freeCam(Cam *cam){
	free(cam);
}
