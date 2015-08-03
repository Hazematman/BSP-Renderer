#ifndef MAT_H
#define MAT_H

#include "types.h"
#include "vec.h"

void identity(mat4 *mat);
void mulMat(mat4 *dst, mat4 *a, mat4 *b);
void frustrum(mat4 *mat, float left, float right, float bottom, float top, float near, float far);
void perspective(mat4 *mat, float fovy, float aspect, float near, float far);
void translate(mat4 *mat, float x, float y, float z);
void rotateY(mat4 *mat, float angle);
void lookAt(mat4 *mat, vec3Float *pos, vec3Float *fwd, vec3Float *right, vec3Float *up);

#endif
