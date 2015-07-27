#include <stdio.h>
#include "pak.h"
#include "bsp.h"
#include "palette.h"
#include "game.h"

int main(){
	if(init() == NO_ERROR){
		run();
	}

	return 0;
}
