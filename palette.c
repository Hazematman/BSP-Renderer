#include <stdlib.h>
#include <string.h>
#include "palette.h"

#define MAX_PALETTE_COLOURS 256

struct s_Palette{
	colour3UByte colours[256];
};

Palette *readPalette(void *paletteData){
	Palette *plt = malloc(sizeof(Palette));
	// Copy palette structure from filedata
	memcpy(&plt->colours[0], paletteData, sizeof(Palette));
	return plt;
}

colour3UByte getPaletteColour(Palette *plt, uint8_t index){
	return plt->colours[index];
}

void freePalette(Palette *plt){
	free(plt);
}
