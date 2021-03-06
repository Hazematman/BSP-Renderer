#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gl/gl_3.h"
#include "bsp.h"
#include "texture.h"
#include "vec.h"

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

typedef struct {
	int16_t planeId;
	
	int16_t side;
	int32_t edgeId;
	int16_t edgeNum;

	int16_t texinfoId;
	uint8_t typeLight;
	uint8_t baseLight;
	uint8_t light[2];
	int32_t lightmap;
} FaceDef;

typedef struct {
	uint16_t vert0;
	uint16_t vert1;
} EdgeDef;

typedef struct {
	vec3Float normal;
	float dist;
	int32_t type;
} PlaneDef;

typedef struct {
	vec3Float vecS;
	float distS;
	vec3Float vecT;
	float distT;
	int32_t texId;
	int32_t animated;
} TexInfoDef;

typedef struct {
	vec3Float position;
	vec2Float texCoord;
	vec3Float normal;
} Vert;

typedef struct {
	GLuint glId;
	Texture *texture;
	int numVerts;
	Vert *verts;
} Face;

struct s_Bsp {
	GLuint vaoId;
	int32_t numTextures;
	Texture **textures;

	vec3Float *verts;
	TexInfoDef *texInfo;
	PlaneDef *planes;

	int numFaces;
	Face *faces;
};

static PlaneDef *getPlane(Bsp *bsp, int planeId){
	return &bsp->planes[planeId];
}

static vec3Float getVert(Bsp *bsp, int vertId){
	return bsp->verts[vertId];
}

static TexInfoDef *getTexInfo(Bsp *bsp, int texInfoId){
	return &bsp->texInfo[texInfoId];
}

static Texture *getTexture(Bsp *bsp, int texId){
	return bsp->textures[texId];
}

static void loadBspPlanes(Bsp *bsp, void *planeData, int32_t planeSize){
	// Load plane data
	bsp->planes = malloc(planeSize);
	memcpy(bsp->planes, planeData, planeSize);
}

static void loadBspVerts(Bsp *bsp, void *vertData, int32_t vertSize){
	// Load vertex positions
	bsp->verts = malloc(vertSize);
	memcpy(bsp->verts, vertData, vertSize);
}

static void loadBspTexInfo(Bsp *bsp, void *texInfoData, int32_t texInfoSize){
	// Load Texture mapping information
	bsp->texInfo = malloc(texInfoSize);
	memcpy(bsp->texInfo, texInfoData, texInfoSize);
}

static void loadBspGeometry(Bsp *bsp, void *faceData, int32_t faceSize, void *edgeData, int32_t edgeSize,
		void *ledgeData, int32_t ledgeSize){

	bsp->numFaces = faceSize / sizeof(FaceDef);
	bsp->faces = malloc(sizeof(Face)*bsp->numFaces);
	FaceDef *faces = malloc(faceSize);
	memcpy(faces, faceData, faceSize);

	EdgeDef *edges = malloc(edgeSize);
	memcpy(edges, edgeData, edgeSize);

	int32_t *ledges = malloc(ledgeSize);
	memcpy(ledges, ledgeData, ledgeSize);

	for(int i=0; i < bsp->numFaces; i++){
		// Read all verts from edge data into a vert data structure
		// that is more suitable for GL rendering
		Face *face = &bsp->faces[i];
		face->numVerts = faces[i].edgeNum;
		face->verts = malloc(sizeof(Vert)*face->numVerts);
		TexInfoDef *texInfo = getTexInfo(bsp, faces[i].texinfoId);
		face->texture = getTexture(bsp, texInfo->texId);
		face->glId = 0;
		// If face is a trigger or clip skip loading it's verts
		// TODO make somekind of stack or vector to store faces so we don't bother
		// allocating useless memory
		if(strcmp("trigger", getTexName(face->texture)) == 0 || strcmp("clip", getTexName(face->texture)) == 0){
			continue;
		}
		for(int edge=0; edge < faces[i].edgeNum; edge++){
			int32_t edgeIndex = ledges[edge+faces[i].edgeId];
			vec3Float verts[2];
			// if edgeIndex is negative flip v0 and v1
			if(edgeIndex < 0){
				edgeIndex = -edgeIndex;
				verts[0] = getVert(bsp, edges[edgeIndex].vert1);
				verts[1] = getVert(bsp, edges[edgeIndex].vert0);
			} else {
				verts[0] = getVert(bsp, edges[edgeIndex].vert0);
				verts[1] = getVert(bsp, edges[edgeIndex].vert1);
			}

			face->verts[edge].normal = getPlane(bsp, faces[i].planeId)->normal;

			// Negate y and flip y & z so it's oriented properly
			face->verts[edge].position.x = verts[0].x;
			face->verts[edge].position.y = verts[0].z;
			face->verts[edge].position.z = -verts[0].y;

			// compute s and t coordinate for texture mapping
			face->verts[edge].texCoord.x = (vec3FDot(&verts[0], &texInfo->vecS) + texInfo->distS) / getTexWidth(face->texture);
			face->verts[edge].texCoord.y = (vec3FDot(&verts[0], &texInfo->vecT) + texInfo->distT) / getTexHeight(face->texture);
		}

		// Now that we have loaded all vertex data for this face
		// We can generate a vbo for it
		glGenBuffers(1, &face->glId);
		glBindBuffer(GL_ARRAY_BUFFER, face->glId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vert)*face->numVerts, &face->verts[0], GL_STATIC_DRAW);
	}

	free(faces);
	free(edges);
	free(ledges);
}

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
		bsp->textures[i] = readPltTex(texDefData + texDef.offset1, texDef.width, texDef.height, plt, texDef.name);
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
	// Start by loading texture data from BSP
	loadBspTextures(bsp, bspData + header.miptex.offset, plt);

	// Next we load the vertex data, plane data, and texture mapping data
	loadBspVerts(bsp, bspData + header.vertices.offset, header.vertices.size);
	loadBspPlanes(bsp, bspData + header.planes.offset, header.planes.size);
	loadBspTexInfo(bsp, bspData + header.texinfo.offset, header.texinfo.size);

	// Next we can load the rest of geometry from the BSP
	loadBspGeometry(bsp, bspData+header.faces.offset, header.faces.size,
			bspData+header.edges.offset, header.edges.size,
			bspData+header.ledges.offset, header.ledges.size);

	// Create vao for face data
	glGenVertexArrays(1, &bsp->vaoId);
	glBindVertexArray(bsp->vaoId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return bsp;
}

void renderAllBspFaces(Bsp *bsp){
	glBindVertexArray(bsp->vaoId);
	for(int f=0; f < bsp->numFaces; f++){
		Face *face = &bsp->faces[f];
		if(face->glId == 0){
			continue;
		}

		glBindBuffer(GL_ARRAY_BUFFER, face->glId);

		Vert *vert = NULL;
		glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vert),
				&vert->position);
		glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vert),
				&vert->texCoord);

		bindTexture(face->texture);

		glDrawArrays(GL_TRIANGLE_FAN, 0, face->numVerts);
	}
}

void freeBsp(Bsp *bsp){
	for(int i=0; i < bsp->numTextures; i++){
		freeTex(bsp->textures[i]);
	}
	for(int i=0; i < bsp->numFaces; i++){
		free(bsp->faces[i].verts);
	}
	free(bsp->faces);
	free(bsp->textures);
	free(bsp->verts);
	free(bsp->planes);
	free(bsp->texInfo);
	free(bsp);
}
