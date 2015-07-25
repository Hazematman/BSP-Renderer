#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "gl/gl_3.h"
#include "timer.h"
#include "game.h"

#define BITS_PER_PIXEL 8
#define DEPTH_BUFFER_SIZE 24
#define STENCIL_BUFFER_SIZE 8
#define MAJOR_GL_VERSION 3
#define MINOR_GL_VERSION 3

#define WINDOW_TITLE "Haze Quake"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static SDL_Window *screen = NULL;
static SDL_GLContext context;

void errMsg(const char *msg){
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!",
			msg, screen);
}

Error init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		errMsg("Could not initialize SDL");
		return INIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BITS_PER_PIXEL);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BITS_PER_PIXEL);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BITS_PER_PIXEL);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BITS_PER_PIXEL);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_BUFFER_SIZE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUFFER_SIZE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_GL_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_GL_VERSION);
	screen = SDL_CreateWindow(WINDOW_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	context = SDL_GL_CreateContext(screen);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED){
		errMsg("Failed to initalize OpenGL functions");
		return INIT_FAILURE;
	}

	return NO_ERROR;
}

void run(){
	bool running = true;
	SDL_Event e;
	
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				running = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(screen);
	}

	return;
}
