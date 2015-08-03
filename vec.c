#include <math.h>
#include "vec.h"

float vec3FDot(vec3Float *v1, vec3Float *v2){
	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

float vec3Len(vec3Float *v){
	return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

void vec3Normalize(vec3Float *v){
	float len = vec3Len(v);
	v->x /= len;
	v->y /= len;
	v->z /= len;
}

void vec3Add(vec3Float *dst, vec3Float *a, vec3Float *b){
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
	dst->z = a->z + b->z;
}

void vec3Sub(vec3Float *dst, vec3Float *a, vec3Float *b){
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
	dst->z = a->z - b->z;
}

void vec3Mul(vec3Float *dst, vec3Float *a, float s){
	dst->x = a->x*s;
	dst->y = a->y*s;
	dst->z = a->z*s;
}

void vec3Cross(vec3Float *dst, vec3Float *a, vec3Float *b){
	vec3Float aa = *a;
	vec3Float bb = *b;

	dst->x = aa.y*bb.z - aa.z*bb.y;
	dst->y = aa.z*bb.x - aa.x*bb.z;
	dst->z = aa.x*bb.y - aa.y*bb.x;
}
