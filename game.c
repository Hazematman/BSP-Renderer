#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "gl/gl_3.h"
#include "timer.h"
#include "game.h"
#include "gutils.h"
#include "mat.h"
#include "pak.h"
#include "bsp.h"
#include "palette.h"

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

static ShaderPrg prg;

static PakFile *pf;
static Palette *plt;
static Bsp *bsp;

static void initGL(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Create required vao
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	createShader(&prg, "data/vs.glsl", "data/fs.glsl");
}

static void initData(){
	pf = readPakFile("id1/pak0.pak");
	plt = readPalette(readFile(pf, "gfx/palette.lmp", NULL));
	bsp = readBsp(readFile(pf, "maps/start.bsp", NULL), plt);
}

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

	initGL();
	initData();

	return NO_ERROR;
}

vec3Float pos;
float speed = 10.0f;

void run(){
	bool running = true;
	SDL_Event e;

	GLint projLoc = getUniformLoc(&prg, "proj");
	GLint transLoc = getUniformLoc(&prg, "trans");
	GLint texLoc = getUniformLoc(&prg, "tex");
	
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				running = false;
			}else if(e.type == SDL_KEYDOWN){
				if(e.key.keysym.sym == SDLK_w){
					pos.z += speed;
				} else if(e.key.keysym.sym == SDLK_a){
					pos.x += speed;
				}
			}
		}

		mat4 proj;
		mat4 trans;
		perspective(&proj, 45.0f * (PI/180.0f), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);
		translate(&trans, pos.x, pos.y, pos.z);
		printf("%f %f %f\n",trans.m[14], trans.m[13], trans.m[12]);
		
		glUseProgram(prg.prgID);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj.m[0]);
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, &trans.m[0]);
		glUniform1i(texLoc, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderAllBspFaces(bsp);
		SDL_GL_SwapWindow(screen);
	}

	SDL_Quit();

	freeBsp(bsp);
	freePalette(plt);
	freePak(pf);
	return;
}
