#ifndef CAM_H
#define CAM_H

#include "types.h"

struct s_Cam;
typedef struct s_Cam Cam;

Cam *makeCam();
void moveCam(Cam *cam, float amount);
void strafeCam(Cam *cam, float amount);
void turnCam(Cam *cam, float anglex, float angley);
void getCamMatrix(Cam *cam, mat4 *mat);
void freeCam(Cam *cam);

#endif
