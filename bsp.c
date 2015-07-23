#include <stdlib.h>
#include "bsp.h"

struct s_Bsp {
};

Bsp *readBsp(void *BspData){
	Bsp *bsp = malloc(sizeof(Bsp));

	return bsp;
}

void freeBsp(Bsp *bsp){
	free(bsp);
}
