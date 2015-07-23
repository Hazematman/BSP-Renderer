#include <stdio.h>
#include "pak.h"
#include "palette.h"

int main(){
	PakFile *pf = readPakFile("id1/pak0.pak");
	Palette *plt = readPalette(readFile(pf, "gfx/palette.lmp", NULL));

	for(int i=0; i<256; i++){
		colourUByte3 c = getPaletteColour(plt, i);
		printf("Colour is %d %d %d\n", c.r, c.g, c.b);
	}

	freePalette(plt);
	freePak(pf);
	return 0;
}
