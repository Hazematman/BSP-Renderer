#include <stdio.h>
#include <stdlib.h>
#include "gl/gl_3.h"
#include "texture.h"
#include "types.h"

struct s_Texture {
	GLuint glId;
	int width;
	int height;
	colour3UByte *data;
};

Texture *readPltTex(uint8_t *texData, int width, int height, Palette *plt){
	Texture *tex = malloc(sizeof(Texture));
	tex->width = width;
	tex->height = height;
	tex->data = malloc(sizeof(colour3UByte)*width*height);

	// Load and convert all paletted colour data	
	for(int i = 0; i < width*height; i++){
		tex->data[i] = getPaletteColour(plt, texData[i]);
	}

	glGenTextures(1, &tex->glId);
	glBindTexture(GL_TEXTURE_2D, tex->glId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			tex->data);


	return tex;
}

int getTexWidth(Texture *tex){
	return tex->width;
}

int getTexHeight(Texture *tex){
	return tex->height;
}

void writePPM(Texture *tex, const char *filename){
	FILE *fp = fopen(filename, "wb");
	if(fp == NULL){
		printf("Could not create file: %s\n", filename);
		return;
	}

	fprintf(fp, "P6 %d %d 255\n", tex->width, tex->height);
	fwrite(tex->data, sizeof(colour3UByte), tex->width*tex->height, fp);
	fclose(fp);
	
	return;	
}

void bindTexture(Texture *tex){
	glBindTexture(GL_TEXTURE_2D, tex->glId);
}

void freeTex(Texture *tex){
	free(tex->data);
	free(tex);
}
