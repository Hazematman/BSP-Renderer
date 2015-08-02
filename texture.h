#ifndef TEXTURE_H
#define TEXTURE_H

#include "palette.h"

struct s_Texture;
typedef struct s_Texture Texture;

Texture *readPltTex(uint8_t *texData, int width, int height, Palette *plt, const char *name);
int getTexWidth(Texture *tex);
int getTexHeight(Texture *tex);
char *getTexName(Texture *tex);
void writePPM(Texture *tex, const char *filename);
void bindTexture(Texture *tex);
void freeTex(Texture *tex);

#endif
