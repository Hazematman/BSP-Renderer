#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp.h"
#include "types.h"

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

struct s_Bsp {
};

Bsp *readBsp(void *BspData){
	BspHeader header;
	
	memcpy(&header, BspData, sizeof(BspHeader));
	if(header.version != BSP_VERSION){
		printf("Invalid BSP version expected %d found %d\n",
				BSP_VERSION, header.version);
		return NULL;
	}

	Bsp *bsp = malloc(sizeof(Bsp));
	return bsp;
}

void freeBsp(Bsp *bsp){
	free(bsp);
}
