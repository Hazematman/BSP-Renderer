#ifndef VEC_H
#define VEC_H

#include "types.h"

float vec3FDot(vec3Float *v1, vec3Float *v2);
float vec3Len(vec3Float *v);
void vec3Normalize(vec3Float *v);
void vec3Add(vec3Float *dst, vec3Float *a, vec3Float *b);
void vec3Sub(vec3Float *dst, vec3Float *a, vec3Float *b);
void vec3Mul(vec3Float *dst, vec3Float *a, float s);
void vec3Cross(vec3Float *dst, vec3Float *a, vec3Float *b);

#endif
