#ifndef TEXTURE_H
#define TEXTURE_H

#include "palette.h"

struct s_Texture;
typedef struct s_Texture Texture;

Texture *readPltTex(uint8_t *texData, int width, int height, Palette *plt);
void writePPM(Texture *tex, const char *filename);
void freeTex(Texture *tex);

#endif
