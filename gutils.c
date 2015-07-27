#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gutils.h"

static char *readFile(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Could not open file: %s\n", filename);
		return NULL;
	}

	// Get size of file
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *data = malloc(fileSize);
	fread(data, 1, fileSize, fp);
	fclose(fp);
	return data;
}

static void printErrLog(GLuint obj){
	GLint logLen = 0;
	bool isShader;
	if(glIsShader(obj)){
		isShader = true;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &logLen);
	} else if (glIsProgram(obj)){
		isShader = false;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &logLen);
	} else {
		printf("Object is neither shader or program");
		return;
	}

	char *log = malloc(logLen);
	if(isShader){
		glGetShaderInfoLog(obj, logLen, NULL, log);
	} else {
		glGetProgramInfoLog(obj, logLen, NULL, log);
	}

	printf("%s", log);
	free(log);
	return;
}

void createShader(ShaderPrg *prg, const char *vs, const char *fs){
	char *vsData = readFile(vs);
	char *fsData = readFile(fs);

	prg->vs = compileShader(vsData, GL_VERTEX_SHADER);
	prg->fs = compileShader(fsData, GL_FRAGMENT_SHADER);

	linkPrg(prg);

	free(vsData);
	free(fsData);
	return;
}

GLuint compileShader(const char *shader, GLenum type){
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &shader, NULL);
	glCompileShader(s);

	// Check if shader compiled okay
	GLint okay = GL_FALSE;
	glGetShaderiv(s, GL_COMPILE_STATUS, &okay);
	if(okay == GL_FALSE){
		printf("Error compiling shader\n");
		printErrLog(s);
		return 0;
	}

	return s;
}

void linkPrg(ShaderPrg *prg){
	prg->prgID = glCreateProgram();
	glAttachShader(prg->prgID, prg->vs);
	glAttachShader(prg->prgID, prg->fs);
	glLinkProgram(prg->prgID);

	// Check if program linked okay
	GLint okay = GL_FALSE;
	glGetProgramiv(prg->prgID, GL_LINK_STATUS, &okay);
	if(okay == GL_FALSE){
		printf("Error linking program\n");
		printErrLog(prg->prgID);
		glDeleteProgram(prg->prgID);
		prg->prgID = 0;
		return;
	}
	return;
}

GLint getAttribLoc(ShaderPrg *prg, const char *name){
	GLint attrib = glGetAttribLocation(prg->prgID, name);
	if(attrib == -1){
		printf("Could not find vertex attribute: %s\n", name);
		return -1;
	}
	
	return attrib;
}

GLint getUniformLoc(ShaderPrg *prg, const char *name){
	GLint uniform = glGetUniformLocation(prg->prgID, name);
	if(uniform == -1){
		printf("Could not find uniform: %s\n", name);
		return -1;
	}

	return uniform;
}
