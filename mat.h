#ifndef MAT_H
#define MAT_H

#include "types.h"

void identity(mat4 *mat);
void frustrum(mat4 *mat, float left, float right, float bottom, float top, float near, float far);
void perspective(mat4 *mat, float fovy, float aspect, float near, float far);
void translate(mat4 *mat, float x, float y, float z);

#endif
