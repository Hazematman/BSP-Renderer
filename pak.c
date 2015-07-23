#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pak.h"

typedef struct {
	char magic[4];
	int32_t offset;
	int32_t size;
} PakHeader;

typedef struct {
	char filename[56];
	int32_t offset;
	int32_t size;
} PakEntry;

typedef struct {
	char *filename;
	int size;
	void *data;
} File;

struct s_PakFile {
	File *files;
	int numFiles;
};

PakFile *readPakFile(const char *filename){
	PakFile *pak = malloc(sizeof(PakFile));
	FILE *fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Could not open file: %s\n", filename);
		return NULL;
	}

	PakHeader header;

	// Read in PAK header
	fread(&header, sizeof(PakHeader), 1, fp);

	// Verify that this is a valid pak file
	if(header.magic[0] != 'P' || header.magic[1] != 'A' ||
			header.magic[2] != 'C' || header.magic[3] != 'K'){
		printf("Invalid pak file: %s\n", filename);
		return NULL;
	}
	
	PakEntry *entries = malloc(header.size);
	int numEntries = header.size/sizeof(PakEntry);
	pak->numFiles = numEntries;
	pak->files = malloc(sizeof(FILE)*numEntries);
	
	// Go to first entry
	fseek(fp, header.offset, SEEK_SET);
	// Read in all entries
	fread(entries, sizeof(PakEntry), numEntries, fp);

	// Read all file entries
	for(int i=0; i < numEntries; i++){
		int filenameLength = strlen(entries[i].filename);
		pak->files[i].filename = malloc(filenameLength+1);
		strcpy(pak->files[i].filename, entries[i].filename);

		pak->files[i].size = entries[i].size;
		pak->files[i].data = malloc(pak->files[i].size);
		fseek(fp, entries[i].offset, SEEK_SET);
		fread(pak->files[i].data, 1, pak->files[i].size, fp);
	}


	fclose(fp);
	return pak;
}

void *readFile(PakFile *pf, const char *filename, int *size){
	for(int i=0; i<pf->numFiles; i++){
		if(strcmp(filename, pf->files[i].filename) == 0){
			// If size pointer isn't NULL set file size
			if(size != NULL){
				*size = pf->files[i].size;
			}
			return pf->files[i].data;
		}
	}

	return NULL;
}

void freePak(PakFile *pf){
	for(int i=0; i<pf->numFiles; i++){
		free(pf->files[i].filename);
		free(pf->files[i].data);
	}
	
	free(pf);
}
