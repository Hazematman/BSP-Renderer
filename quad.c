#include "quad.h"

static float verts[] = {
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
};

static float texCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};

static GLuint bufferIds[2];

void initQuad(){
	glGenBuffers(2, bufferIds);
	glBindBuffer(GL_ARRAY_BUFFER, bufferIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), &texCoords[0], GL_STATIC_DRAW);
}

void drawQuad(ShaderPrg *prg){
	GLint pos = getAttribLoc(prg, "position");
	GLint tex = getAttribLoc(prg, "texCoord");
	glEnableVertexAttribArray(pos);
	glBindBuffer(GL_ARRAY_BUFFER, bufferIds[0]);
	glVertexAttribPointer(
			pos,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);

	glEnableVertexAttribArray(tex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]);
	glVertexAttribPointer(
			tex,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableVertexAttribArray(pos);
	glDisableVertexAttribArray(tex);
}
