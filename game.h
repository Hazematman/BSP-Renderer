#ifndef GAME_H
#define GAME_H

typedef enum {
	NO_ERROR,
	INIT_FAILURE,
} Error;

Error init();
void run();

void errMsg(const char *msg);

#endif
