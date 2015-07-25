#include <stdio.h>
#include "pak.h"
#include "bsp.h"
#include "palette.h"

int main(){
	PakFile *pf = readPakFile("id1/pak0.pak");
	Palette *plt = readPalette(readFile(pf, "gfx/palette.lmp", NULL));
	
	Bsp *bsp = readBsp(readFile(pf, "maps/start.bsp", NULL), plt);

	freeBsp(bsp);
	freePalette(plt);
	freePak(pf);
	return 0;
}
