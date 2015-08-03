#include <math.h>
#include "mat.h"

void identity(mat4 *mat){
	float *m = &mat->m[0];
	m[0] = 1;m[4] = 0;m[8] = 0;m[12]= 0;
	m[1] = 0;m[5] = 1;m[9] = 0;m[13]= 0;
	m[2] = 0;m[6] = 0;m[10]= 1;m[14]= 0;
	m[3] = 0;m[7] = 0;m[11]= 0;m[15]= 1;
}

void mulMat(mat4 *dst, mat4 *a, mat4 *b){
	mat4 aa = *a;
	mat4 bb = *b;
	float *dm = &dst->m[0];
	float *am = &aa.m[0];
	float *bm = &bb.m[0];

	for(int j=0; j<4; j++){
		for(int i=0; i<4; i++){
			int index = j*4 + i;
			dm[index] = am[0*4 + i]*bm[j*4 + 0] + am[1*4 + i]*bm[j*4 + 1]
			+ am[2*4 + i]*bm[j*4 + 2] + am[3*4 + i]*bm[j*4 + 3]; 
		}
	}
}

void frustrum(mat4 *mat, float left, float right, float bottom, float top, float near, float far){
	mat4 r;
	float *m = &r.m[0];
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

	mulMat(mat, mat, &r);
}

void perspective(mat4 *mat, float fovy, float aspect, float near, float far){
	mat4 r;
	float *m = &r.m[0];
	float range = tanf(fovy)*near;
	float sx = (2.0f*near)/(range*aspect + range*aspect);
	float sy = near/range;
	float sz = -(far + near)/(far - near);
	float pz = -(2.0f*far*near)/(far - near);

	m[0] = sx;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = sy;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10]= sz;
	m[11]= -1;

	m[12]= 0;
	m[13]= 0;
	m[14]= pz;
	m[15]= 0;

	mulMat(mat, mat, &r);
}

void translate(mat4 *mat, float x, float y, float z){
	mat4 r;
	float *m = &r.m[0];
	m[0] = 1;m[4] = 0;m[8] = 0;m[12]= x;
	m[1] = 0;m[5] = 1;m[9] = 0;m[13]= y;
	m[2] = 0;m[6] = 0;m[10]= 1;m[14]= z;
	m[3] = 0;m[7] = 0;m[11]= 0;m[15]= 1;


	mulMat(mat, mat, &r);
}

void rotateY(mat4 *mat, float angle){
	mat4 r;
	float *m = &r.m[0];
	m[0] = cosf(angle);
	m[1] = 0;
	m[2] = -sinf(angle);
	m[3] = 0;

	m[4] = 0;
	m[5] = 1;
	m[6] = 0;
	m[7] = 0;

	m[8] = sinf(angle);
	m[9] = 0;
	m[10]= cosf(angle);
	m[11]= 0;

	m[12]= 0;
	m[13]= 0;
	m[14]= 0;
	m[15]= 1;

	mulMat(mat, mat, &r);
}

void lookAt(mat4 *mat, vec3Float *pos, vec3Float *fwd, vec3Float *right, vec3Float *up){
	mat4 r;
	float *m = &r.m[0];

	m[0] = right->x;
	m[1] = up->x;
	m[2] = -fwd->x;
	m[3] = 0;

	m[4] = right->y;
	m[5] = up->y;
	m[6] = -fwd->y;
	m[7] = 0;	

	m[8] = right->z;
	m[9] = up->z;
	m[10]= -fwd->z;
	m[11]= 0;

	m[12]= -pos->x;
	m[13]= -pos->y;
	m[14]= -pos->z;
	m[15]= 1;

	mulMat(mat, mat, &r);
}
