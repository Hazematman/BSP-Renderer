#include <stdio.h>
#include <stdlib.h>
#include "texture.h"
#include "types.h"

struct s_Texture {
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

	return tex;
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

void freeTex(Texture *tex){
	free(tex->data);
	free(tex);
}
