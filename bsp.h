#ifndef BSP_H
#define BSP_H

struct s_Bsp;
typedef struct s_Bsp Bsp;

Bsp *readBsp(void *BspData);
void freeBsp(Bsp *bsp);

#endif
