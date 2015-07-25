#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp.h"
#include "texture.h"

#define BSP_VERSION 29

typedef struct {
	int32_t offset;
	int32_t size;
} BspEntry;

typedef struct {
	int32_t version;
	BspEntry entities;
	BspEntry planes;

	BspEntry miptex;
	BspEntry vertices;

	BspEntry visilist;
	BspEntry nodes;

	BspEntry texinfo;

	BspEntry faces;

	BspEntry lightmaps;
	BspEntry clipnodes;

	BspEntry leaves;

	BspEntry lface;
	BspEntry edges;

	BspEntry ledges;
	BspEntry models;
} BspHeader;

typedef struct {
	char name[16];
	uint32_t width;
	uint32_t height;
	uint32_t offset1;
	uint32_t offset2;
	uint32_t offset4;
	uint32_t offset8;
} TextureDef;

struct s_Bsp {
	int32_t numTextures;
	Texture **textures;
};

static void loadBspTextures(Bsp *bsp, void *texData, Palette *plt){
	// Read in number of textures
	memcpy(&bsp->numTextures, texData, sizeof(int32_t));
	bsp->textures = malloc(sizeof(Texture*)*bsp->numTextures);

	// For each texture read its offset and then read texture data
	int32_t *offsetData = texData + sizeof(int32_t);
	for(int i=0; i < bsp->numTextures; i++){
		int32_t offset = offsetData[i];

		void *texDefData = texData + offset;
		TextureDef texDef;
		memcpy(&texDef, texDefData, sizeof(TextureDef));

		// Load and convert the texture to RGB
		// TODO: deal with mipmaps
		bsp->textures[i] = readPltTex(texDefData + texDef.offset1, texDef.width, texDef.height, plt);
	}

	return;
}

Bsp *readBsp(void *bspData, Palette *plt){
	BspHeader header;
	
	memcpy(&header, bspData, sizeof(BspHeader));
	if(header.version != BSP_VERSION){
		printf("Invalid BSP version expected %d found %d\n",
				BSP_VERSION, header.version);
		return NULL;
	}

	Bsp *bsp = malloc(sizeof(Bsp));
	loadBspTextures(bsp, bspData + header.miptex.offset, plt);
	return bsp;
}

void freeBsp(Bsp *bsp){
	free(bsp);
}
