#ifndef GUTILS_H
#define GUTILS_H

#include "gl/gl_3.h"

typedef struct {
	GLuint prgID;
	GLuint vs, fs;
} ShaderPrg;

void createShader(ShaderPrg *prg, const char *vs, const char *fs);
GLuint compileShader(const char *shader, GLenum type);
void linkPrg(ShaderPrg *prg);

GLint getAttribLoc(ShaderPrg *prg, const char *name);
GLint getUniformLoc(ShaderPrg *prg, const char *name);

#endif
