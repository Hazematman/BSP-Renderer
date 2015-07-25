#include <stdio.h>
#include "pak.h"
#include "bsp.h"
#include "palette.h"
#include "game.h"

int main(){
	if(init() == NO_ERROR){
		PakFile *pf = readPakFile("id1/pak0.pak");
		Palette *plt = readPalette(readFile(pf, "gfx/palette.lmp", NULL));
		
		Bsp *bsp = readBsp(readFile(pf, "maps/start.bsp", NULL), plt);
		run();
		freeBsp(bsp);
		freePalette(plt);
		freePak(pf);
	}

	return 0;
}
