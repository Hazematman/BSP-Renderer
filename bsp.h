#ifndef BSP_H
#define BSP_H

#include "palette.h"

struct s_Bsp;
typedef struct s_Bsp Bsp;

Bsp *readBsp(void *bspData, Palette *plt);
void renderAllBspFaces(Bsp *bsp);
void freeBsp(Bsp *bsp);

#endif
