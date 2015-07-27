#include <math.h>
#include "mat.h"

void identity(mat4 *mat){
	float *m = &mat->m[0];
	m[0] = 1;m[4] = 0;m[8] = 0;m[12]= 0;
	m[1] = 0;m[5] = 1;m[9] = 0;m[13]= 0;
	m[2] = 0;m[6] = 0;m[10]= 1;m[14]= 0;
	m[3] = 0;m[7] = 0;m[11]= 0;m[15]= 1;
}

void frustrum(mat4 *mat, float left, float right, float bottom, float top, float near, float far){
	float *m = &mat->m[0];
	float n2 = 2.0*near;
	float rml = right - left;
	float fmn = far - near;
	float tmb = top - bottom;

	m[0] = n2/rml;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = n2/tmb;
	m[6] = 0;
	m[7] = 0;
	
	m[8] = (right + left)/rml;
	m[9] = (top + bottom)/tmb;
	m[10]= -(far + near)/fmn;
	m[11]= -1;

	m[12]= 0;
	m[13]= 0;
	m[14]= -n2/fmn;
	m[15]= 0;
}

void perspective(mat4 *mat, float fovy, float aspect, float near, float far){
	float ymax = near * tanf(fovy);
	float xmax = ymax * aspect;
	frustrum(mat, -xmax, xmax, -ymax, ymax, near, far);
}

void translate(mat4 *mat, float x, float y, float z){
	float *m = &mat->m[0];
	m[0] = 1;m[4] = 0;m[8] = 0;m[12]= x;
	m[1] = 0;m[5] = 1;m[9] = 0;m[13]= y;
	m[2] = 0;m[6] = 0;m[10]= 1;m[14]= z;
	m[3] = 0;m[7] = 0;m[11]= 0;m[15]= 1;
}
