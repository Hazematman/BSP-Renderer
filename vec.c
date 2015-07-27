#include "vec.h"

float vec3FDot(vec3Float *v1, vec3Float *v2){
	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}
